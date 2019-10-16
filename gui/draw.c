#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define GL_SILENCE_DEPRECATION 1

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

void *work(void *args);

#define NOGL 0

#if NOGL
#else
#include <pthread.h>
#include <GLFW/glfw3.h>
#define AUTO_REFRESH 60
#define OFFSET 64
#define WIDTH 256
#define HEIGHT 256
#define IM_W 32
#define IM_H 32

static GLFWwindow* window;
u8 gl_ok=0;
u8 flip_y=1;
u8 new_frame=1;
u8 limit_speed=0;
u32 cyclelimit = 0;
u8 debugmode = 0;
u8 show_debug = 0;
u8 pix[3*IM_W*IM_H];

double t0; // global start time
double get_time() {
  struct timeval tv; gettimeofday(&tv, NULL);
  return (tv.tv_sec + tv.tv_usec * 1e-6);
}

int nsleep(long ms) {
  struct timespec req, rem;
  if (ms > 999) {
    req.tv_sec = (int)(ms/1000);
    req.tv_nsec = (ms - ((long)req.tv_sec*1000)) * 1000000;
  }
  else {
    req.tv_sec=0;
    req.tv_nsec=ms*1000000;
  }
  return nanosleep(&req, &rem);
}
#define bind_key(x,y) \
{ if (action == GLFW_PRESS && key == (x)) (y) = 1; if (action == GLFW_RELEASE && key == (x)) (y) = 0; if (y) {printf(#y "\n");} }

static void error_callback(int error, const char* description) { }
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_0) show_debug ^= 1;
    if (action == GLFW_PRESS && key == GLFW_KEY_9) limit_speed ^= 1;
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

u8 mouse_hold = 0;

static void mouse_pos_callback(GLFWwindow* window, double x, double y) {
  int xpos, ypos, width, height;
  glfwGetWindowPos(window, &xpos, &ypos);
  glfwGetWindowSize(window, &width, &height);
  if (1==mouse_hold) {
    int ix = (int)(((double)IM_W*x)/(double)WIDTH);
    int iy = (int)(-0.5+((double)IM_H*y)/(double)HEIGHT);
    pix[0+ix*3+iy*IM_W*3] += 4;
    pix[1+ix*3+iy*IM_W*3] += 4;
    pix[2+ix*3+iy*IM_W*3] += 4;
  }
  printf("x=%f, y=%f, hold=%d\n", x, y, mouse_hold);
}

static void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods) {
  //if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int x = (int)(((double)IM_W*xpos)/(double)WIDTH);
    int y = (int)(-0.5+((double)IM_H*ypos)/(double)HEIGHT);
    printf("click at (x=%f, y=%f): (x=%d, y=%d)\n", xpos, ypos, x, y);
    mouse_hold = 1;
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    mouse_hold = 0;
  }
}

static GLFWwindow* open_window(const char* title, GLFWwindow* share, int posX, int posY)
{
    GLFWwindow* window;

    //glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, share);
    if (!window) return NULL;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowPos(window, posX, posY);
    glfwShowWindow(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_btn_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);

    return window;
}

static void draw_quad()
{
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float incr_x = 1.0f/(float)IM_W; float incr_y = 1.0f/(float)IM_H;
    glOrtho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
    glBegin(GL_QUADS);
    float i,j;
    u8 px,py;
    for (u16 x=0; x<IM_W; x++) for (u16 y=0; y<IM_H; y++ ) {
         i = x * incr_x; j = y * incr_y; px = x; py = flip_y ? IM_H - y - 1 : y; // FLIP vert

          glColor4f(pix[3*(px+py*IM_W)+0]/255.0f,
                    pix[3*(px+py*IM_W)+1]/255.0f,
                    pix[3*(px+py*IM_W)+2]/255.0f,
                    255.0f/255.0f);

          glVertex2f(i,      j     );     glVertex2f(i+incr_x, j     );
          glVertex2f(i+incr_x, j+incr_y); glVertex2f(i,      j+incr_y);
    }
    glEnd();
};

int display_init() {
  int x, y, width;
  //glfwSetErrorCallback(error_callback);
  if (!glfwInit()) return -1;

  window = open_window("NES", NULL, OFFSET, OFFSET);
  if (!window) { glfwTerminate(); return -1; }

  glfwGetWindowPos(window, &x, &y);
  glfwGetWindowSize(window, &width, NULL);
  glfwMakeContextCurrent(window);

  gl_ok=1;
  printf("%9.6f, GL_init OK\n", get_time()-t0);

  double frame_start=get_time();
  while (!glfwWindowShouldClose(window))
  {
    double fps = 1.0/(get_time()-frame_start);
    char wtitle[256]; wtitle[255] = '\0';
    frame_start = get_time();
    sprintf(wtitle, "%4.1f FPS", fps);
    glfwSetWindowTitle(window, wtitle);
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_quad();
    glfwSwapBuffers(window);
    if (AUTO_REFRESH > 0) glfwWaitEventsTimeout(1.0/(double)AUTO_REFRESH - (get_time()-frame_start));
    else glfwWaitEvents();
  }

  glfwTerminate();
  printf("%9.6f, GL terminating\n", get_time()-t0);
  gl_ok=0;
  return 0;
}
#endif

int main(int argc, char **argv) {

  #if NOGL
    work(argv[1]);
  #else
    pthread_t work_thread;
    if(pthread_create(&work_thread, NULL, work, argv[1])) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
    }

    display_init();

    if(pthread_join(work_thread, NULL)) {
      fprintf(stderr, "Error joining thread\n");
      return 2;
    }

  #endif

  return 0;
}

void *work(void *args) {

  int i = 0;

  while (!gl_ok) nsleep(10);
  {
    while (gl_ok) {
      //printf("working %d\n", i++);
      nsleep(100);
      //pix[i % (IM_W * IM_H * 3)] = rand() % 256;
    }

  }
  return 0;
}
