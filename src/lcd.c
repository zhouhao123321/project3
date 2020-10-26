#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close  	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

#include "lcd.h"

#include "jpeglib.h"


static unsigned char g_color_buf[FB_SIZE]={0};

int  lcd_fd;
int *mmap_fd;



//初始化LCD
int lcd_open(void)
{
	lcd_fd = open("/dev/fb0", O_RDWR);
	
	if(lcd_fd<0)
	{
			printf("open lcd error\n");
			return -1;
	}

	mmap_fd  = (int *)mmap(	NULL, 					//映射区的开始地址，设置为NULL时表示由系统决定映射区的起始地址
									FB_SIZE, 				//映射区的长度
									PROT_READ|PROT_WRITE, 	//内容可以被读取和写入
									MAP_SHARED,				//共享内存
									lcd_fd, 				//有效的文件描述词
									0						//被映射对象内容的起点
								);
	return lcd_fd;

}

//LCD画点
// void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color)
// {
// 	*(mmap_fd+y*800+x)=color;
// }
// rgb转换完了之后在运用这个宏在这个上显示这个颜色
// 本来是一个函数的，为了不让这个代码移植调用这个函数，就把它定义成一个宏，
// 估计在内存上更方便
#define  lcd_draw_point(x,y,color) \
	*(mmap_fd+y*800+x)=color;



//显示摄像头捕捉
int show_video_data(unsigned int x,unsigned int y,char *pjpg_buf,unsigned int jpg_buf_size)  
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	unsigned char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	//unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	
			//  int	lcd_buf[480][800] = {0};
	//		 int	jpg_fd;
	
	//unsigned int 	jpg_width;
	//unsigned int 	jpg_height;
	

		
	pjpg = pjpg_buf;

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_buf_size);
	
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	
	x_e	= x_s+cinfo.output_width;
	y_e	= y  +cinfo.output_height;	

	/*读解码数据*/
	while(cinfo.output_scanline < cinfo.output_height )
	{		
		pcolor_buf = g_color_buf;
			
		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo,&pcolor_buf,1);
		
		for(i=0; i<cinfo.output_width; i++)
		{
			/* 获取rgb值 */
			color = 		*(pcolor_buf+2);
			color = color | *(pcolor_buf+1)<<8;
			color = color | *(pcolor_buf)<<16;
			
			/* 显示像素点 */
			lcd_draw_point(x,y,color);
			// lcd_buf[y][x] = color ;
			
			pcolor_buf +=3;
			
			x++;
		}
		
		/* 换行 */
		y++;			
		
		x = x_s;
		
	}	


			
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return 0;
}







int lcd_draw_jpg2(unsigned int x,unsigned int y,const char *pjpg_path,int flag_t) 
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	unsigned char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	//unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	unsigned int	y_n	= y;
	unsigned int	x_n	= x;
	
			//  int	lcd_buf[480][800] = {0};
			 int	jpg_fd;
	unsigned int 	jpg_size;


	if(pjpg_path!=NULL)
	{
		/* 申请jpg资源，权限可读可写 */	
		jpg_fd=open(pjpg_path,O_RDWR);
		
		if(jpg_fd == -1)
		{
		   printf("open %s error\n",pjpg_path);
		   
		   return -1;	
		}	
		
		/* 获取jpg文件的大小 */
		jpg_size=file_size_get(pjpg_path);	
		if(jpg_size<3000)
			return -1;
		
		/* 为jpg文件申请内存空间 */	
		pjpg = malloc(jpg_size);

		/* 读取jpg文件所有内容到内存 */		
		read(jpg_fd,pjpg,jpg_size);
	}
	else
	{
		printf("jpeg path error : %s \n", pjpg_path );
		return -1;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_size);
	
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);

	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	
	
	x_e	= x_s +cinfo.output_width;
	y_e	= y  +cinfo.output_height;	
	int num_height = 0;
	int num_width = 0;

	int flag=0;
	double x_b=1;//x，y的比例
	double y_b=1;
	char lcd_buf[480][800]={0};
	/*读解码数据*/
	while(cinfo.output_scanline < cinfo.output_height )
	{		
		pcolor_buf = g_color_buf;
		
		/* 读取jpg一行的rgb值 */
		jpeg_read_scanlines(&cinfo,&pcolor_buf,1);
		
		for(i=0; i<cinfo.output_width; i++)
		{

			/* 获取rgb值 */
			color = 		*(pcolor_buf+2);
			color = color | *(pcolor_buf+1)<<8;
			color = color | *(pcolor_buf)<<16;	 
			
			/* 显示像素点 */
			if (flag_t == 1)
			{		
				if (cinfo.output_width >=LCD_WIDTH)
				{
					num_width = i*LCD_WIDTH/cinfo.output_width;//缩小长
				}				
				if (cinfo.output_height >=LCD_HEIGHT)
				{
					num_height = y_n*LCD_HEIGHT/cinfo.output_height;
				}
			}
			if (flag_t == 1)   //自动缩放
			{	
				if(flag!=1)
				{
					lcd_draw_point(num_width,num_height,color);
				}
				else
				{
					lcd_draw_point(x_n,y_n,color);	
				}
						
			}
			else
			{
				lcd_draw_point(x_n,y_n,color);	
			}
				 //lcd_buf[y_n][x_n] = color ;

			pcolor_buf +=3;
			
			x_n++;
		}
		
		/* 换行 */
		y_n++;			
		
		x_n = x_s;
		
	}


			
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(pjpg_path!=NULL)
	{
		/* 关闭jpg文件 */
		close(jpg_fd);	
		
		/* 释放jpg文件内存空间 */
		free(pjpg);		
	}

	return 0;
}



//显示正常jpg图片                                           1.jpg
int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path)  
{
	/*定义解码对象，错误处理对象*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	// 比如这些结构体定义在jpeg库解压的文件里面，在解压库的路径里面
	// vim example.c 里面能看到很多头文件，写jpg文件 定义结构体，	这里面有demo也就是例子
	// 你可以仿着去写
	// 定义一些变量
	unsigned char 	*pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	//unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	unsigned int	y_n	= y;
	unsigned int	x_n	= x;
	
			//  int	lcd_buf[480][800] = {0};
			 int	jpg_fd;
	unsigned int 	jpg_size;

//    如果为空  
	if(pjpg_path!=NULL)
	{
		/* 申请jpg资源，权限可读可写 */	
		jpg_fd=open(pjpg_path,O_RDWR);
		// 不为空直接打开
		
		if(jpg_fd == -1)
		{
		   printf("open %s error\n",pjpg_path);
		   
		   return -1;	
		}	
		
		/* 获取jpg文件的大小 */
		// 获取文件大小，其实下面这个函数调用的是另一个函数
		jpg_size=file_size_get(pjpg_path);	
		if(jpg_size<3000)
			return -1;
// jpg图片的尺寸信息，来获取文件大小，为什么要获取文件大小，因为我们要创建一个空间
// 来放这个jpg图片的信息，因为jpg图片的大小是不确定的，要根据实际的像素点来压缩图片
// 压缩出来的图片大小是不确定

// 为jpg申请空间
		/* 为jpg文件申请内存空间 */	
		pjpg = malloc(jpg_size);

// 读取jpg里面的内容到港申请的空间里面
		/* 读取jpg文件所有内容到内存 */		
		read(jpg_fd,pjpg,jpg_size);
	}
	// 为空直接打印错误
	else  
	{
		printf("jpeg path error : %s \n", pjpg_path );
		return -1;
	}

	/*注册出错处理*/
	cinfo.err = jpeg_std_error(&jerr);

	/*创建解码*/
	jpeg_create_decompress(&cinfo);

	/*直接解码内存数据*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_size);
	// 下面读取文件头就和哦我们读bmp的文件头信息相似，
	// 文件头会告诉你，则是什么图片采用了什么方式压缩你应该怎么解压
	// 文件大小是多大少，在下面的参数cinfo里面
	/*读文件头*/
	jpeg_read_header(&cinfo, TRUE);
// 解码时一行一行的解码
	/*开始解码*/
	jpeg_start_decompress(&cinfo);	
	
	// 以上跟刚刚我们访问的vim exampl.c里面的demo一样
	
	x_e	= x_s +cinfo.output_width;
	y_e	= y  +cinfo.output_height;	
// 当前读的这一行有没有超出你的行高度
	/*读解码数据*/
	while(cinfo.output_scanline < cinfo.output_height )
	{		
		pcolor_buf = g_color_buf;
		
		/* 读取jpg一行的rgb值 */
		// 先读取一行，读完了之后再将你读的今昔那个rgb 转换
		jpeg_read_scanlines(&cinfo,&pcolor_buf,1);
		
		for(i=0; i<cinfo.output_width; i++)
		{
			/* 不显示的部分 */
			/* if(y_n>g_jpg_in_jpg_y && y_n<g_jpg_in_jpg_y+240)
				if(x_n>g_jpg_in_jpg_x && x_n<g_jpg_in_jpg_x+320)
				{
					pcolor_buf +=3;		
					x_n++;			
					continue;
				} */
				
			/* 获取rgb值 */
			// 这就是rgb 转换蓝色值左移八位，绿色左移八位，红色左移16为
			color = 		*(pcolor_buf+2);
			color = color | *(pcolor_buf+1)<<8;
			color = color | *(pcolor_buf)<<16;	
			
			/* 显示像素点 */
			if (x_n <= 799 && y_n <= 479)
			{
				// 显示完一个颜色再将pcolor_buf +3;移到下一个像素点去
				lcd_draw_point(x_n,y_n,color);
				// lcd_buf[y_n][x_n] = color ;
			}
			
			
			
			pcolor_buf +=3;
			
			x_n++;
		}
		
		/* 换行 */
		y_n++;			
		// x_s是起始位置，x_n是结束位置
		x_n = x_s;
		
	}		


			
	/*解码完成*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(pjpg_path!=NULL)
	{
		/* 关闭jpg文件 */
		close(jpg_fd);	
		
		/* 释放jpg文件内存空间 */
		free(pjpg);		
	}

	return 0;
}


//LCD关闭
void lcd_close(void)
{
	
	/* 取消内存映射 */
	munmap(mmap_fd, FB_SIZE);
	
	/* 关闭LCD设备 */
	close(lcd_fd);
}

//获取jpg文件的大小
// 它调用的是stat这个函数，stat这个函数运用了一个结构体，这个结构体有着
// jpg图片的尺寸信息，来获取文件大小，为什么要获取文件大小，因为我们要创建一个空间
// 来放这个jpg图片的信息，因为jpg图片的大小是不确定的，要根据实际的像素点来压缩图片
// 压缩出来的图片大小是不确定
unsigned long file_size_get(const char *pfile_path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	
	if(stat(pfile_path, &statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	
	return filesize;
}

