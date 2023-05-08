#include "main.h"

ext_client_custom_graphic_double_t lane;
ext_client_custom_graphic_double_t bead;

void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
{
    unsigned char ucCRC = 0;
    if((pchMessage == 0) || (dwLength <= 2))
    {
        return;
    }
    ucCRC = get_CRC8_check_sum((unsigned char *)pchMessage, dwLength - 1, CRC8_INIT);
    pchMessage[dwLength - 1] = ucCRC;
}
void Append_CRC16_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
{
    unsigned short wCRC = 0;
    if ((pchMessage == 0) || (dwLength <= 2))
    {
        return;
    }
    wCRC = get_CRC16_check_sum((unsigned char *)pchMessage, dwLength - 2, CRC16_INIT);
    pchMessage[dwLength - 2] = (unsigned char)(wCRC & 0x00ff);
    pchMessage[dwLength - 1] = (unsigned char)((wCRC >> 8) & 0x00ff);
}

void Usart3_Sent_Byte(uint8_t ch)
{
    USART3->DR = ch;
    while ((USART3->SR & 0x40) == 0)
        ;
}

void Usart3_Sent_string(uint8_t *string, uint16_t length)
{
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        Usart3_Sent_Byte(string[i]);
    }
}

void Client_Sent_String(uint8_t *string, uint16_t length)
{
    Usart3_Sent_string(string, length);
}

void figure_graphic(graphic_data_struct_t *graphic, // 最终要发出去的数组的数据段内容
                    const char *name,
                    uint32_t operate_tpye,
                    uint32_t graphic_tpye, // 绘制什么图像
                    uint32_t layer,
                    uint32_t color,
                    uint32_t start_angle,
                    uint32_t end_angle,
                    uint32_t width,
                    uint32_t start_x,
                    uint32_t start_y,
                    uint32_t radius,
                    uint32_t end_x,
                    uint32_t end_y)
{
    for (uint8_t i = 0; i < 3; i++)
        graphic->graphic_name[i] = name[i]; // 字符索引
    graphic->operate_tpye = operate_tpye;   // 图层操作
    graphic->graphic_tpye = graphic_tpye;   // Char型
    graphic->layer = layer;                 // 都在第一层
    graphic->color = color;                 // 变色
    graphic->start_angle = start_angle;
    graphic->end_angle = end_angle;
    graphic->width = width;
    graphic->start_x = start_x;
    graphic->start_y = start_y;
    graphic->radius = radius;
    graphic->end_x = end_x;
    graphic->end_y = end_y;
}

void draw_lane()
{
    figure_graphic(&lane.grapic_data_struct[0], "l0", ADD, LINE, 0, WHITE, 0, 0, 5, 100, 200, 0, 240, 240);
    figure_graphic(&lane.grapic_data_struct[1], "l1", ADD, LINE, 0, WHITE, 0, 0, 5, 1920 - 100, 1080 - 200, 0, 240, 240);
}

void draw_bead(uint32_t x, uint32_t y)
{
    figure_graphic(&bead.grapic_data_struct[0], "b0", ADD, CIRCLE, 1, WHITE, 0, 0, 3, 0, 0, 0, 240, 240);
    figure_graphic(&bead.grapic_data_struct[1], "b1", ADD, CIRCLE, 1, WHITE, 0, 0, 3, 0, 0, 0, 240, 240);
}

uint8_t CliendTxBuffer[200];

void bead_draw() // 两个个图像一起更新
{
    // 帧头
    bead.txFrameHeader.SOF = HEADER_SOF;
    bead.txFrameHeader.data_length = sizeof(ext_client_data_header_t) + sizeof(graphic_data_struct_t) * 2;
    bead.txFrameHeader.seq = 0; // 包序号
    memcpy(CliendTxBuffer, &bead.txFrameHeader, sizeof(STUDENT_INTERACTIVE_DATA_CMD_ID));
    Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(frame_header_struct_t)); // 头校验

    // 命令码
    bead.CmdID = STUDENT_INTERACTIVE_DATA_CMD_ID;

    // 数据段头结构
    bead.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_one_graphic;
    bead.dataFrameHeader.send_ID = robinfo.robid;
    bead.dataFrameHeader.receiver_ID = robinfo.robid;

    // 数据段
    draw_bead(middle_x, middle_y);
    memcpy(CliendTxBuffer + LEN_FRAME_HEAD, (uint8_t *)&bead.CmdID, LEN_CMD_ID + bead.txFrameHeader.data_length); // 加上命令码长度2

    // 帧尾
    Append_CRC16_Check_Sum(CliendTxBuffer, sizeof(bead));

    Client_Sent_String(CliendTxBuffer, sizeof(bead));
}

void UIdrawer()
{
    for (;;)
    {
        bead_draw();
        osDelayUntil(UIdrawperi);
    }
}