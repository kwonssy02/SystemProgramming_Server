ifndef _CAMERA_H_
#define _CAMERA_H_

int cameraloc();
static int close_buffers(struct SecBuffer *buffers, int num_of_buf);
static int get_pixel_depth(unsigned int fmt);
static int fimc_poll(struct pollfd *events);
static int fimc_v4l2_querycap(int fp);
static const __u8* fimc_v4l2_enuminput(int fp, int index);
static int fimc_v4l2_s_input(int fp, int index);
static int fimc_v4l2_s_fmt(int fp, int width, int height, unsigned int fmt, enum v4l2_field field, unsigned int num_plane);
static int fimc_v4l2_s_fmt_cap(int fp, int width, int height, unsigned int fmt);
int fimc_v4l2_s_fmt_is(int fp, int width, int height, unsigned int fmt, enum v4l2_field field);
static int fimc_v4l2_enum_fmt(int fp, unsigned int fmt);
static int fimc_v4l2_reqbufs(int fp, enum v4l2_buf_type type, int nr_bufs);
static int fimc_v4l2_querybuf(int fp, struct SecBuffer *buffers, enum v4l2_buf_type type, int nr_frames, int num_plane);
static int fimc_v4l2_streamon(int fp);
static int fimc_v4l2_streamoff(int fp);
static int fimc_v4l2_qbuf(int fp, int index );
static int fimc_v4l2_dqbuf(int fp, int num_plane);
static int fimc_v4l2_g_ctrl(int fp, unsigned int id);
static int fimc_v4l2_s_ctrl(int fp, unsigned int id, unsigned int value);
static int fimc_v4l2_s_ext_ctrl(int fp, unsigned int id, void *value);
static int fimc_v4l2_g_parm(int fp, struct v4l2_streamparm *streamparm);
static int fimc_v4l2_s_parm(int fp, struct v4l2_streamparm *streamparm);
int CreateCamera(int index);
void DestroyCamera();
int startPreview(void);
int stopPreview(void);
void initScreen(unsigned char *fb_mem );
static void yuv_a_rgb(unsigned char y, unsigned char u, unsigned char v,
               unsigned char* r, unsigned char* g, unsigned char* b);
static void Draw(unsigned char *displayFrame, unsigned char *videoFrame,int videoWidth, int videoHeight, \
				 int dFrameWidth,int dFrameHeight);
static void DrawFromRGB565(unsigned char *displayFrame, unsigned char *videoFrame,int videoWidth, int videoHeight, \
				 int dFrameWidth,int dFrameHeight);








enum scenario_mode {
    IS_MODE_PREVIEW_STILL,
    IS_MODE_PREVIEW_VIDEO,
    IS_MODE_CAPTURE_STILL,
    IS_MODE_CAPTURE_VIDEO,
    IS_MODE_MAX
};
endif


