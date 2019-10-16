#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define GL_SILENCE_DEPRECATION 1

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define MAX_POINTS 128
int points_len = 0;
struct point {
  int x;
  int y;
} points[MAX_POINTS];

void func(void *args) {
  int v = *((int*)args);
  printf("func %d\n", v);
}

void insert_point(int x, int y) {
  if (points_len < MAX_POINTS) {
    points[points_len++] = (struct point){ .x=x, .y=y };
  }
}

void remove_point() {
  if (points_len > 0) {
    points_len--;
  }
}

void *work(void *args);

#define NOGL 0

#if NOGL
#else
#include <pthread.h>
#include <GLFW/glfw3.h>
#define AUTO_REFRESH 60
#define OFFSET 64
#define IM_W 128
#define IM_H 128

static GLFWwindow* window;
u8 gl_ok=0;
u8 flip_y=1;
u8 new_frame=1;
u8 limit_speed=0;
u32 cyclelimit = 0;
u8 debugmode = 0;
u8 show_debug = 0;
u8 pix[3*IM_W*IM_H];

int closest = -1;
int show_lines = 0;

int WIDTH = 512;
int HEIGHT = 512;

void generate_data() {

  while (points_len > 0) remove_point();

  for (int i = 0; i < MAX_POINTS; ++i) {
    insert_point(i, rand() % IM_H);
  }
}

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

int mx=-1;
int my=-1;

static void change_size_callback(GLFWwindow* window, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
}

static void error_callback(int error, const char* description) { }
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_R) generate_data();
    if (action == GLFW_PRESS && key == GLFW_KEY_L) show_lines ^= 1;
    if (action == GLFW_PRESS && key == GLFW_KEY_I) insert_point(mx,my);
    if (action == GLFW_PRESS && key == GLFW_KEY_U) remove_point();
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void mouse_pos_callback(GLFWwindow* window, double x, double y) {
  int xpos, ypos, width, height;
  glfwGetWindowPos(window, &xpos, &ypos);
  glfwGetWindowSize(window, &width, &height);
  mx = (int)(((double)IM_W*x)/(double)WIDTH);
  my = (int)(-0.5+((double)IM_H*y)/(double)HEIGHT);
  //printf("(%d, %d)\n", mx, my);
}

static void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int x = (int)(((double)IM_W*xpos)/(double)WIDTH);
    int y = (int)(-0.5+((double)IM_H*ypos)/(double)HEIGHT);
    printf("closest %d\n", closest);
    func(&closest);
    //printf("click at x=%.3f (%d), y=%.3f (%d)\n", xpos, x, ypos, y);
    //insert_point(x, y);
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
    glfwSetWindowSizeCallback(window, change_size_callback);

    return window;
}

void draw_point(int x, int y, float r, float g, float b, float a) {

    float incr_x = 1.0f/(float)IM_W; float incr_y = 1.0f/(float)IM_H;
    glColor4f(r, g, b, a);
    float i = x * incr_x;
    float j = y * incr_y;
    j = flip_y ? 1-j : j; // FLIP vert
    glVertex2f(i,      j     );     glVertex2f(i+incr_x, j     );
    glVertex2f(i+incr_x, j+incr_y); glVertex2f(i,      j+incr_y);
}

void draw_line(int x0, int y0, int x1, int y1, float r, float g, float b, float a, float w) {

    glLineWidth(w);
    float incr_x = 1.0f/(float)IM_W; float incr_y = 1.0f/(float)IM_H;
    glColor4f(r, g, b, a);
    float i0 = x0 * incr_x;
    float j0 = y0 * incr_y;
    float i1 = x1 * incr_x;
    float j1 = y1 * incr_y;
    j0 = flip_y ? 1-j0 : j0; // FLIP vert
    j1 = flip_y ? 1-j1 : j1; // FLIP vert
    glVertex2f(i0, j0);
    glVertex2f(i1, j1);
}

static void draw_quad()
{
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
    glBegin(GL_QUADS);

    for (int i = 0; i < points_len; ++i)
      draw_point(points[i].x, points[i].y, 0, 1, 0, 0.75);

    // cursor
    draw_point(mx, my, 1, 0, 0, 0.5);
    glEnd();

    glBegin(GL_LINES);

    int ii = -1;
    double min_d = 0.0;
    for (int i = 0; i < points_len; ++i) {
      double d = sqrt((points[i].x-mx)*(points[i].x-mx)+(points[i].y-my)*(points[i].y-my));
      if (ii < 0 || d < min_d ) {
        ii = i; min_d = d;
      }
    }
    if (points_len > 0) {
      closest = ii;
      draw_line(mx, my, points[ii].x, points[ii].y, 0, 0, 1, 0.5, 2);
    }
    glEnd();

    if (1 == show_lines) {
      glBegin(GL_LINES);
      for (int i = 0; i < points_len; ++i) {
        draw_line(points[i-1].x, points[i-1].y, points[i].x, points[i].y, 0, 0.5, 0.5, 0.5, 1);
      }
      glEnd();
    }
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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  gl_ok=1;
  printf("%9.6f, GL_init OK\n", get_time()-t0);

  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
  glEnable(GL_BLEND);

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
      nsleep(100);
      //for (int i = 0; i < points_len; ++i) {
      //  int x = points[i].x;
      //  int y = points[i].y;
      //  printf("%3d: x=%3d, y=%3d, d=%3.3f\n", i, mx, my, d);
      //}
    }

  }
  return 0;
}
