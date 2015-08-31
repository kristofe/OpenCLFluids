
//GRID DIMENSIONS
#define NX 64
#define NY 64
#define NZ 1
#define _H_  1.0f



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>


#elif WIN32
#include "GL/glut.h"
#include <Windows.h>

#else
#include <GL/glut.h>
#endif


#include "second_order_solver.h"


float dt;
float force, source;
int dvel;
int dobs;

float * g_u, * g_v, * g_w, * g_u_prev, * g_v_prev, * g_w_prev;
float * g_dens, * g_dens_prev;
float * g_heat, * g_heat_prev;
float * g_curl;
float * g_compressibility;
float * g_divergence;
float * g_pressure, *g_pressure_prev;
float * g_laplacian_matrix;
float * g_cg_r, *g_cg_d, *g_cg_q;

int * g_obs;

int win_id;
int win_x, win_y;
int mouse_down[3];
int omx, omy, mx, my;

int step;
int maccormack;
int vorticity;
int useCG;
int simpleInput;
int addForces;

//OpenCL globals
int dims[3] = { NX, NY, NZ};

static void free_data ( void )
{
    if ( g_u ) free ( g_u );
    if ( g_v ) free ( g_v );
    if ( g_w ) free ( g_w );
    if ( g_u_prev ) free ( g_u_prev );
    if ( g_v_prev ) free ( g_v_prev );
    if ( g_w_prev ) free ( g_w_prev );
    if ( g_dens ) free ( g_dens );
    if ( g_dens_prev ) free ( g_dens_prev );
    if ( g_curl ) free ( g_curl );
    if ( g_obs ) free ( g_obs );
    if ( g_heat ) free (g_heat);
    if ( g_heat_prev) free(g_heat_prev);
    if (g_compressibility) free (g_compressibility);
    if (g_divergence) free (g_divergence);
    if (g_pressure) free (g_pressure);
    if (g_pressure_prev) free (g_pressure_prev);
  if (g_laplacian_matrix) free (g_laplacian_matrix);
  if (g_cg_r) free (g_cg_r);
  if (g_cg_d) free (g_cg_d);
  if (g_cg_q) free (g_cg_q);


}

static void clear_data ( void )
{
    int i, size=NX*NY*NZ;

    for ( i=0 ; i<size ; i++ ) {
        g_u[i] = g_v[i] = g_w[i] = g_u_prev[i] = g_v_prev[i] = g_w_prev[i] =
            g_dens[i] = g_dens_prev[i] = g_curl[i] =
            g_heat[i] = g_heat_prev[i] = g_compressibility[i] =
            g_divergence[i] = g_pressure[i] = g_pressure_prev[i] =
            g_heat[i] = g_heat_prev[i]= 0.0f;
    }

  //memset(g_laplacian_matrix,0.0f, sizeof(float)*size*size);

    //particle_system->clear();
}

static int allocate_data ( void )
{
    int size = NX*NY*NZ;

    g_u				= (float *) malloc ( size*sizeof(float) );
    g_v				= (float *) malloc ( size*sizeof(float) );
    g_w				= (float *) malloc ( size*sizeof(float) );
    g_u_prev		= (float *) malloc ( size*sizeof(float) );
    g_v_prev		= (float *) malloc ( size*sizeof(float) );
    g_w_prev		= (float *) malloc ( size*sizeof(float) );
    g_dens			= (float *) malloc ( size*sizeof(float) );
    g_dens_prev		= (float *) malloc ( size*sizeof(float) );
    g_curl			= (float *) malloc ( size*sizeof(float) );
    g_heat			= (float *) malloc ( size*sizeof(float) );
    g_heat_prev		= (float *) malloc ( size*sizeof(float) );
    g_divergence	= (float *) malloc ( size*sizeof(float) );
    g_pressure		= (float *) malloc ( size*sizeof(float) );
    g_pressure_prev	= (float *) malloc ( size*sizeof(float) );
    g_compressibility =  (float *) malloc ( size*sizeof(float) );
    g_obs   		= (int *) calloc ( size,sizeof(int) );

  g_cg_r =  (float *) malloc ( size*sizeof(float) );
    g_cg_d =  (float *) malloc ( size*sizeof(float) );
    g_cg_q =  (float *) malloc ( size*sizeof(float) );

    if ( !g_u || !g_v || !g_u_prev || !g_v_prev || !g_dens || !g_dens_prev || !g_curl || !g_compressibility
        || !g_pressure || !g_pressure_prev || !g_divergence || !g_obs) {
        fprintf ( stderr, "cannot allocate data\n" );
        return ( 0 );
    }

    //particle_system = new CombustionParticleSystem(100,win_x,win_y,N);
  g_laplacian_matrix = (float *) malloc ( size*size*sizeof(float) );



    return ( 1 );
}

static void setup_sources_and_forces()
{  
  const int ww = NX*0.2;
  const int hh = 2;
  int left = NX / 2 - ww / 2;
  int right = NX / 2 + ww / 2;
  int bottom = 1;
  int top = bottom + hh;

  FOR_EACH_CELL
  {

    if (i > left  && i < right  && j > bottom && j < top)
    {
      int idx = IX(i, j, 0);
      g_dens_prev[idx] = 1.0f;
      g_heat_prev[idx] = g_heat[idx] = 0.1f;
    }
  }

}
static void setup_obstacles()
{  
  //setup obstacles
  const int ww = NX*0.125;
  const int hh = NY*0.0125;
  int left = NX / 2 - ww / 2 + NX / 4;
  int right = NX / 2 + ww / 2 + NX / 4;
  int top = NY / 2 + hh / 2;
  int bottom = NY / 2 - hh / 2;

  float3 center = { NX / 2.0f, NY / 6.0f, 0.0f };
  float radius = NX / 20.0f;
  FOR_EACH_CELL
  {
    float3 pos = { i, j, 0 };
    float3 diff = { center.x - i, center.y - j, 0 };
    float distSqr = diff.x*diff.x + diff.y*diff.y;
    if (distSqr < radius*radius)
    {
      g_obs[IX(i, j, 0)] = 1;
    }

    /*
    if (i > left  && i < right  && j > bottom && j < top)
      g_obs[IX(i, j, 0)] = 1;
      */
  }

}

static void pre_display ( void )
{
    glViewport ( 0, 0, win_x, win_y );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    gluOrtho2D ( 0.0, 1.0, 0.0, 1.0 );
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT );
}

static void post_display ( void )
{
    glutSwapBuffers ();
}

static void draw_velocity ( void )
{
    int i, j;
    float x, y, h;

    h = 1.0f/NX;

    glColor3f ( 1.0f, 1.0f, 1.0f );
    glLineWidth ( 1.0f );

    glBegin ( GL_LINES );

        for ( i=0 ; i<NX ; i++ ) {
            //x = (i-0.5f)*h;
            x = (i+0.5f)*h;

            for ( j=0 ; j<NY ; j++ ) {
                //y = (j-0.5f)*h;
                y = (j+0.5f)*h;

                glColor3f(1.0,1.0,1.0);
                glVertex2f ( x, y );
                glColor3f(1.0,0.0,0.0);
                glVertex2f ( x+g_u[IX(i,j,0)], y+g_v[IX(i,j,0)] );
            }
        }

    glColor3f(1.0,1.0,1.0);
    glEnd ();
}

static void draw_pressure ( void )
{
    int i, j;
    float x, y, h, d00, d01, d10, d11;

    h = 1.0f/NX;

    glBegin ( GL_QUADS );

        for ( i=0 ; i<NX ; i++ ) {
            x = (i-0.0f)*h;
            for ( j=0 ; j<NY ; j++ ) {
                y = (j-0.0f)*h;

                d00 = 255.0f * fabs(get_data(g_divergence,i,j,0));
                d01 = 255.0f * fabs(get_data(g_divergence,i,j+1,0));
                d10 = 255.0f * fabs(get_data(g_divergence,i+1,j,0));
                d11 = 255.0f * fabs(get_data(g_divergence,i+1,j+1,0));

                glColor3f ( d00, d00, d00 ); glVertex2f ( x, y );
                glColor3f ( d10, d10, d10 ); glVertex2f ( x+h, y );
                glColor3f ( d11, d11, d11 ); glVertex2f ( x+h, y+h );
                glColor3f ( d01, d01, d01 ); glVertex2f ( x, y+h );
            }
        }

    glEnd ();
}

static void draw_density ( void )
{
    int i, j;
    float x, y, h, d00, d01, d10, d11;

    h = 1.0f/NX;

    glBegin ( GL_QUADS );

        for ( i=0 ; i<NX ; i++ ) {
            x = (i-0.0f)*h;
            for ( j=0 ; j<NY ; j++ ) {
                y = (j-0.0f)*h;

                d00 = get_data(g_dens,i,j,0);
                d01 = get_data(g_dens,i,j+1,0);
                d10 = get_data(g_dens,i+1,j,0);
                d11 = get_data(g_dens,i+1,j+1,0);

                glColor3f ( d00, d00, d00 ); glVertex2f ( x, y );
                glColor3f ( d10, d10, d10 ); glVertex2f ( x+h, y );
                glColor3f ( d11, d11, d11 ); glVertex2f ( x+h, y+h );
                glColor3f ( d01, d01, d01 ); glVertex2f ( x, y+h );
            }
        }

    glEnd ();
}

static void draw_heat(void)
{
  int i, j;
  float x, y, h, d00, d01, d10, d11;

  h = 1.0f / NX;

  glBegin(GL_QUADS);

  for (i = 0; i < NX; i++) {
    x = (i - 0.0f)*h;
    for (j = 0; j < NY; j++) {
      y = (j - 0.0f)*h;

      d00 = get_data(g_heat, i, j, 0);
      d01 = get_data(g_heat, i, j + 1, 0);
      d10 = get_data(g_heat, i + 1, j, 0);
      d11 = get_data(g_heat, i + 1, j + 1, 0);

      glColor4f(1.0f, 0.0f, 0.0f, d00); glVertex2f(x, y);
      glColor4f(1.0f, 0.0f, 0.0f, d10); glVertex2f(x + h, y);
      glColor4f(1.0f, 0.0f, 0.0f, d11); glVertex2f(x + h, y + h);
      glColor4f(1.0f, 0.0f, 0.0f, d01); glVertex2f(x, y + h);
    }
  }

  glEnd();
}


static void draw_obstacles ( void )
{
    int i, j;
    float x, y, h, d00, d01, d10, d11;

    h = 1.0f/NX;

    glBegin ( GL_QUADS );

        for ( i=0 ; i<NX ; i++ ) {
            x = (i-0.0f)*h;
            for ( j=0 ; j<NY ; j++ ) {
                y = (j-0.0f)*h;

                int v = get_int_data(g_obs, i, j, 0);
                if(v == 0) continue;
                d00 = 255.0f *v;


                glColor3f ( 0, d00, d00 );
                glVertex2f ( x, y );
                glVertex2f ( x+h/2, y );
                glVertex2f ( x+h/2,  y+h/2 );
                glVertex2f ( x, y+h/2 );
            }
        }

    glEnd ();
}
/*
  ----------------------------------------------------------------------
   relates mouse movements to forces sources
  ----------------------------------------------------------------------
*/

//static void get_from_UI ( float * d, float * u, float * v, float * heat, float * compressibility, CombustionParticleSystem* cps )
static void get_from_UI ( float * d, float * u, float * v, float * heat, float * compressibility)
{
    int i, j, size = NX*NY*NZ;

    /*
    for ( i=0 ; i<size ; i++ ) {
        u[i] = v[i] = w[i] = d[i] = 0.0f;
    }
    */
    if (addForces)
      setup_sources_and_forces();

    if ( !mouse_down[0] && !mouse_down[2] ) return;

    i = (int)((       mx /(float)win_x)*NX);
    j = (int)(((win_y-my)/(float)win_y)*NY);

    if ( i<2 || i>=NX-1 || j<2 || j>=NY-1 ) return;

    if (mouse_down[2])
    {
      g_heat_prev[IX(i, j, 0)] = 1.0f;
    }

    if ( mouse_down[0] || mouse_down[2] ) {
        /*
        compressibility[IX(i,j)] = 1.0f;
        compressibility[IX(i+1,j)] = 1.0f;
        compressibility[IX(i,j+1)] = 1.0f;
        compressibility[IX(i+1,j+1)] = 1.0f;
        heat[IX(i,j)] = 0.05f;
     */
      if (mouse_down[2] || simpleInput){
        d[IX(i, j, 0)] = source;
      }

      if (mouse_down[0] || simpleInput)
      {
        float diffx = mx - omx;
        float diffy = win_y - my - omy;

        if (abs(diffx) > 0.000001)
          diffx = diffx / diffx * diffx<0.0f ? -1.0f : 1.0f;
        if (abs(diffy) > 0.000001)
          diffy = diffy / diffy * diffy < 0.0f ? -1.0f : 1.0f;

        u[IX(i, j, 0)] = 0.1f * force * diffx;
        v[IX(i, j, 0)] = 0.1f * force * diffy;

        //		u[IX(i-1,j,0)] = 0.075f *  force;
        //		u[IX(i,j,0)] = 0.075f *  force;
        //		u[IX(i+1,j,0)] = 0.075f *  force;
        //		v[IX(i,j+1,0)] = 0.5f * force * diffy;
      }
    }

    /*
    if ( mouse_down[2] ) {
        cps->trigger(mx, my, 100);
        //d[IX(i,j)] = source;
    }
    */

    omx = mx;
    omy = win_y-my;
    
}

/*
  ----------------------------------------------------------------------
   GLUT callback routines
  ----------------------------------------------------------------------
*/

static void key_func ( unsigned char key, int x, int y )
{
    switch ( key )
    {
        case 'x':
        case 'X':
            clear_data ();
            break;

        case 'q':
        case 'Q':
        case 27: // ESCAPE KEY
            free_data ();
            exit ( 0 );
            break;

        case 'v':
        case 'V':
            dvel = !dvel;
            break;

        case 'b':
        case 'B':
            dobs = !dobs;
            break;

        case 's':
        case 'S':
            step = 1;
            break;
        case 'c':
        case 'C':
            useCG = !useCG;
            printf("Using %s solver\n",useCG?"Conjugate Gradient":"Jacobi");
            break;

        case 'm':
        case 'M':
            maccormack = !maccormack;
            printf("MacCormack = %s\n",maccormack?"true":"false");
            break;

        case 'o':
        case 'O':
            vorticity = !vorticity;
            printf("Vorticity = %s\n",vorticity?"true":"false");
            break;
        case 'i':
        case 'I':
            simpleInput = !simpleInput;
            printf("Simple Input = %s\n",simpleInput?"true":"false");
            break;
        case 'f':
        case 'F':
            addForces = !addForces;
            printf("Add Forces= %s\n",addForces?"true":"false");
            break;
    }
}

static void mouse_func ( int button, int state, int x, int y )
{
    omx = mx = x;
    omy = my = y;

    mouse_down[button] = state == GLUT_DOWN;
}

static void motion_func ( int x, int y )
{
    mx = x;
    my = y;
}

static void reshape_func ( int width, int height )
{
    glutSetWindow ( win_id );
    glutReshapeWindow ( width, height );

    win_x = width;
    win_y = height;
    //particle_system->update_win(win_x,win_xy);
}

static void idle_func ( void )
{
    //if(step)
    {
        get_from_UI ( g_dens_prev, g_u_prev, g_v_prev, g_heat,g_compressibility);

        //blur(g_u_prev,g_v_prev,g_w_prev, dt);


    if(maccormack){
            advect_velocity_maccormack(dt, g_u, g_v, g_w, g_u_prev, g_v_prev, g_w_prev, g_obs);
        } else {
            //advect_velocity_forward_euler(dt, g_u, g_v, g_w, g_u_prev, g_v_prev, g_w_prev);
            advect_velocity_RK2(dt, g_u, g_v, g_w, g_u_prev, g_v_prev, g_w_prev, g_obs);
        }

    //Bouyance should be considered an external force and should be applied immediately after velocity advection
    bouyancy(g_v, g_heat);
    //Need to advect heat
    advectRK2(dt, g_heat, g_heat_prev,g_u, g_v, g_w,g_obs);

    //Maybe damp it too

    //project(dt,g_u,g_v, g_w, g_divergence, g_pressure, g_pressure_prev);
    project(dt,g_u,g_v, g_w, g_divergence, g_pressure, g_pressure_prev, g_laplacian_matrix,g_cg_r, g_cg_d, g_cg_q,g_obs,useCG);


   advectRK2(dt,g_dens,g_dens_prev, g_u, g_v, g_w, g_obs);

    if(vorticity) {
            vorticity_confinement(dt, g_u, g_v, g_w, g_u_prev, g_v_prev, g_w_prev);
        }

    

        SWAP(g_u,g_u_prev);
        SWAP(g_v,g_v_prev);
        SWAP(g_dens, g_dens_prev);
        SWAP(g_heat, g_heat_prev);

        //copy_grid(g_u,g_u_prev);
        //copy_grid(g_v,g_v_prev);
        //copy_grid(g_dens, g_dens_prev);

        step = 0;
    }

    glutSetWindow ( win_id );
    glutPostRedisplay ();



}

static void display_func ( void )
{
    pre_display ();



    if ( dvel )
    {
      draw_pressure();
      draw_velocity ();
    }
        else
    {
      draw_density ();
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      draw_heat();
      glDisable(GL_BLEND);
    }

    if ( dobs )
    {
      draw_obstacles();
    }

    post_display ();
}







void readMatrix(float* m, int n){
  FILE *fp;
  fp = fopen("spd-mat.txt","r");
  //if(fp != NULL)
    //printf("file opened\n");
  // For the rest, read in the values
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      fscanf(fp, "%e", &m[i+j*n]);
      //printf("%3.6f ", m[i+j*n]);
    }
    //printf("\n");
  }


  fclose(fp);
}



#ifndef WIN32
void testCG(){
  int N = 100;
  //read n=100 spd matrix from file
  float A[N*N];
  readMatrix(A, N);

  //run cg on it to see if it converges


  //When porting cg to opencl  only the mtx-vec multiply and the
  //dot-product(reduction) are run on the GPU everything is on the GPU
  int i = 0;
  int imax = 1000;
  float tol = 0.000001f;
  float r[N];
  float b[N];
  float d[N];
  float x[N];
  float q[N];

  for(int i = 0; i < N; i++){
    x[i] = 0.0f;
    b[i] = rand()/(float)RAND_MAX * 100.0f;
    r[i] = b[i];
    d[i] = r[i];
  }

  float rnew = dot(r,r,N);
  float rold = 0.0f;

  float r0 = rnew;
  //while(i < imax && rnew > 0.0000001*r0) {
  while(i < imax && rnew > tol) {
    mtx_times_vec(q,A,d,N);
    float alpha = rnew/(dot(d,q,N));

    for(int j = 0; j < N; j++){
      x[j] += alpha*d[j];
    }

    for(int j = 0; j < N; j++){
      r[j] -= alpha*q[j];
    }

    rold = rnew;
    rnew = dot(r,r,N);

    float beta = rnew/rold;

    for(int j = 0; j < N; j++){
      d[j] = r[j] + beta*d[j];
    }

    i++;
  }

  //Check the answer
  float ax[N];
  int goodMatrix = 1;
  mtx_times_vec(ax,A,x,N);
  for(int i = 0; i < N; i++){
    float diff = ax[i] - b[i];
    if(fabs(diff) > 0.01f){
      goodMatrix = 0;
      printf("CG Result mismatch at idx %d ax[%d] = %3.6f != b[%d] = %3.6f\n", i,i,ax[i],i,b[i]);
    }
  }
  if(goodMatrix){
    printf("CG Converged and Solved Ax=b\n");
  }else{
    printf("CG did not solve Ax=b\n");
  }

  printf("CG Terminated with iterations %d, and rnew %3.6f\n",i, rnew);



}
#endif //WIN32


static void open_glut_window ( void )
{
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowPosition ( 0, 0 );
    glutInitWindowSize ( win_x, win_y );
  win_id = glutCreateWindow ( "Fluids" );



    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT );
    glutSwapBuffers ();
    glClear ( GL_COLOR_BUFFER_BIT );
    glutSwapBuffers ();

    pre_display ();

    glutKeyboardFunc ( key_func );
    glutMouseFunc ( mouse_func );
    glutMotionFunc ( motion_func );
    glutReshapeFunc ( reshape_func );
    glutIdleFunc ( idle_func );
    glutDisplayFunc ( display_func );
}

int main ( int argc, char ** argv )
{
  //testCG();
  win_x = 512;
    win_y = 512;


    glutInit ( &argc, argv );

    open_glut_window ();

  //test_opencl_opengl_interop();


  dt = 0.1f;
  force = 10.0f;
  source = 10.0f;


  printf ( "\n\nHow to use this demo:\n\n" );
  printf ( "\t Add densities with the left mouse button\n" );
  printf ( "\t Add velocities with the left mouse button and dragging the mouse\n" );
  printf ( "\t Toggle density/velocity display with the 'v' key\n" );
  printf ( "\t toggle obstacles display by pressing the 'b' key\n" );
  printf ( "\t Clear the simulation by pressing the 'x' key\n" );
  printf ( "\t switch poisson solvers from jacobi to conjugate gradient by pressing the 'c' key\n" );
  printf ( "\t switch advection scheme from RK2 to MacCormack by pressing the 'm' key\n" );
  printf ( "\t toggle vorticity confinement by pressing the 'o' key\n" );
  printf ( "\t toggle simple input by pressing the 'i' key.  This combines density and velocity input on a left mouse drag.\n" );
  printf(  "\t toggle automatic adding of sources and forces by pressing the 'f' key\n");

  printf ( "\t Quit by pressing the 'q' key\n" );

  dvel = 0;
  dobs = 1;

  step = 0;
  maccormack = 0;
  vorticity = 1;
  useCG = 1;
  simpleInput = 1;
  addForces = 1;

  if ( !allocate_data () ) exit ( 1 );
  clear_data ();



  //setupMatrix(g_laplacian_matrix);

//	FOR_EACH_FACE
//	{
//		//if(i < NX - NX*0.4 && i > NX*0.4
//		//	&&
//		//   j < NY - NY*0.4 && j > NY*0.4 )
//		{
//			g_u_prev[IX(i,j,0)] =  -0.01 * cosf(3.14159 * 2.0 * i/NX);
//			g_v_prev[IX(i,j,0)] =  0.01 * sinf(3.14159 * 2.0 * j/NY);
//		}
//	}


  copy_grid(g_u_prev, g_u);
  copy_grid(g_v_prev, g_v);

  //g_dens_prev[IX(16,3,0)] = 10.0f;


  setup_obstacles();

  //g_u_prev[IX(16,3,0)] = 10.0f;

    /*
    calculate_divergence(g_divergence, g_u_prev, g_v_prev, g_w_prev, dt);
    pressure_solve(g_pressure,g_pressure_prev, g_divergence, dt);
    pressure_apply(g_u_prev, g_v_prev, g_w_prev, g_pressure, dt);
    //project(dt,g_u_prev,g_v_prev, g_w_prev, g_divergence, g_pressure, g_pressure_prev);
    SWAP(g_u_prev,g_u);
    SWAP(g_v_prev,g_v);
    SWAP(g_w_prev,g_w);

    if(!check_divergence(g_u_prev, g_v_prev, g_w_prev))
    {
        printf("Initial field wasn't divergence free!\n");
    }
    */



//	run_tests();






    glutMainLoop ();

    exit ( 0 );
}

