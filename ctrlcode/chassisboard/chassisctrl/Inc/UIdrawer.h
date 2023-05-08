#ifndef UIDRAWER_H
#define UIDRAWER_H

#define middle_x 960
#define middle_y 540
typedef struct
{
    uint8_t operate_tpye;
    uint8_t layer;
} __attribute__((packed)) ext_client_custom_graphic_delete_t;

typedef struct
{
    uint8_t graphic_name[3];
    uint32_t operate_tpye : 3;
    uint32_t graphic_tpye : 3;
    uint32_t layer : 4;
    uint32_t color : 4;
    uint32_t start_angle : 9;
    uint32_t end_angle : 9;
    uint32_t width : 10;
    uint32_t start_x : 11;
    uint32_t start_y : 11;
    uint32_t radius : 10;
    uint32_t end_x : 11;
    uint32_t end_y : 11;
} __attribute__((packed)) graphic_data_struct_t;

typedef struct //数据段内容格式
{ 
	uint16_t data_cmd_id;    
	uint16_t send_ID;    
	uint16_t receiver_ID; 
}__attribute__((packed)) ext_client_data_header_t; 

typedef struct
{
    frame_header_struct_t txFrameHeader;
    uint16_t CmdID;
    ext_client_data_header_t dataFrameHeader;
    graphic_data_struct_t grapic_data_struct[2];
    uint16_t FrameTail;
} __attribute__((packed)) ext_client_custom_graphic_double_t;

typedef struct
{
    frame_header_struct_t txFrameHeader;
    uint16_t CmdID;
    ext_client_data_header_t dataFrameHeader;
    graphic_data_struct_t grapic_data_struct[5];
    uint16_t FrameTail;
} __attribute__((packed)) ext_client_custom_graphic_five_t;

typedef struct
{
    frame_header_struct_t txFrameHeader;
    uint16_t CmdID;
    ext_client_data_header_t dataFrameHeader;
    graphic_data_struct_t grapic_data_struct[7];
    uint16_t FrameTail;
} __attribute__((packed)) ext_client_custom_graphic_seven_t;

enum judge_data_length_t {
	/* Std */
	LEN_FRAME_HEAD 	                 = 5,	// 帧头长度
	LEN_CMD_ID 		                   = 2,	// 命令码长度
	LEN_FRAME_TAIL 	                 = 2,	// 帧尾CRC16
	/* Ext */
	// 0x000x
	LEN_GAME_STATUS 				         = 11,
	LEN_GAME_RESULT 				         = 1,
	LEN_GAME_ROBOT_HP 			         = 28,
	LEN_DART_STATUS					         = 3,
	LEN_ICRA_BUFF_DEBUFF_ZONE_STATUS = 11,//0x0005
	// 0x010x
	LEN_EVENT_DATA					         = 4,
	LEN_SUPPLY_PROJECTILE_ACTION	   = 4,//！！！！！！！！！！！！！！！！！
	LEN_SUPPLY_PROJECTILE_BOOKING	   = 3,//对抗赛未开启
	LEN_REFEREE_WARNING				       = 2,
	LEN_DART_REMAINING_TIME		     	 = 1,//0x0105
	// 0x020x
	LEN_GAME_ROBOT_STATUS			       = 27,//15!!!!!!!!!!!!!!!!!!!!!!!!!!!
	LEN_POWER_HEAT_DATA 			       = 16,//！！！！！！！！！！
	LEN_GAME_ROBOT_POS				       = 16,
	LEN_BUFF_MASK		 				         = 1,
	LEN_AERIAL_ROBOT_ENERGY 	     	 = 1,//！！！！！
	LEN_ROBOT_HURT				         	 = 1,
	LEN_SHOOT_DATA					         = 7,//！！！！
	LEN_BULLET_REMAINING	 		       = 6,//！！！！
	LEN_RFID_STATUS					         = 4,
	LEN_DART_CLIENT_DIRECTIVE        = 12,//0x020A
	// 0x030x
	//LEN_robot_interactive_header_data      = n,
	//LEN_controller_interactive_header_data = n,
	LEN_MAP_INTERACTIVE_HEADERDATA           = 15,
	LEN_KEYBOARD_INFORMATION                 = 12,//0x0304
};//表2-4

enum
{
	//0x200-0x02ff 	队伍自定义命令 格式  INTERACT_ID_XXXX
	INTERACT_ID_delete_graphic 			= 0x0100,	/*客户端删除图形*/
	INTERACT_ID_draw_one_graphic 		= 0x0101,	/*客户端绘制一个图形*/
	INTERACT_ID_draw_two_graphic 		= 0x0102,	/*客户端绘制2个图形*/
	INTERACT_ID_draw_five_graphic 	= 0x0103,	/*客户端绘制5个图形*/
	INTERACT_ID_draw_seven_graphic 	= 0x0104,	/*客户端绘制7个图形*/
	INTERACT_ID_draw_char_graphic 	= 0x0110,	/*客户端绘制字符图形*/
	INTERACT_ID_bigbome_num					= 0x02ff
};

enum
{
	LEN_INTERACT_delete_graphic     = 8,  //删除图层 2(数据内容ID)+2(发送者ID)+2（接收者ID）+2（数据内容）  
	LEN_INTERACT_draw_one_graphic   = 21, // 以上2+2+2+15
	LEN_INTERACT_draw_two_graphic   = 36, //6+15*2
	LEN_INTERACT_draw_five_graphic  = 81, //6+15*5
	LEN_INTERACT_draw_seven_graphic = 111,//6+15*7
	LEN_INTERACT_draw_char_graphic  = 51, //6+15+30（字符串内容）
};

typedef enum
{
	NONE   = 0,/*空操作*/
	ADD    = 1,/*增加图层*/
	MODIFY = 2,/*修改图层*/
	DELETE = 3,/*删除图层*/
}Graphic_Operate;//graphic_data_struct_t：uint32_t operate_tpye
/*图层操作*/
//bit3-5
typedef enum
{
	LINE      = 0,//直线
	RECTANGLE = 1,//矩形
	CIRCLE    = 2,//正圆
	OVAL      = 3,//椭圆
	ARC       = 4,//圆弧
	INT    	  = 5,//浮点数
	FLOAT     = 6,//整型数
	CHAR      = 7 //字符
}Graphic_Type;
/*图层类型*/
//bit 6-9图层数 最大为9，最小0
//bit 10-13颜色
typedef enum
{
	RED_BLUE  = 0,//红蓝主色	
	YELLOW    = 1,
	GREEN     = 2,
	ORANGE    = 3,
	FUCHSIA   = 4,	/*紫红色*/
	PINK      = 5,
	CYAN_BLUE = 6,	/*青色*/
	BLACK     = 7,
	WHITE     = 8
}Graphic_Color;

#endif