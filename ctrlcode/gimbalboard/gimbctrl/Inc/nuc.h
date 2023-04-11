#ifndef NUC_H
#define NUC_H

#define NUC_RX_BUF_NUM   20u
#define NUC_FRAME_LENGTH 10u

void NUC_INIT(void);

typedef struct
{
    float yaw;
    float pitch;

} NUC_DATA;

extern NUC_DATA NUC_data;

#endif // !NUC_H
