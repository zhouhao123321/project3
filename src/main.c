#include "font.h"
#include "main.h"
#include <cJSON.h>

// #include "./inc/cJSON.h"
// #include "./inc/main.h"
// #include "./inc/font.h"
#include <netdb.h>
#define DEBUG


struct LcdDevice *init_lcd(const char *device);
void show_chat(char * name);
void show_weather(char * name,int x,int y,int chang,int gao);
int flag = 0;
char *tmp;
char *tmp2;
char rbuf1[4096*10];
int Shut_down();
char *p1;
char *p2;
int  weather();
int chark_tmd(char * name);
int cheak_weather(char * name);
int picture_download();
int famous_sayings();
int translate();
void sig(int arg);//信号处理函数
typedef struct city
{
    char id[20];
    char name[50];

}city;
enum weather
{
    QING = 0,
    DUOYU,
    DAYU,
    XIAOYU,
    YIN,
    BAOYU,
    ZHENYU
} W;

pthread_t tid;
bool find_city_id(char *cmd, city *c);
#define      CHU_1    "/dev/input/event0"

int cheak_weather(char * name)
{
    if(!strcmp(name,"晴"))
    {
        return QING;
    }
    if(strstr(name,"小雨"))
    {
        return XIAOYU;
    }
    if(!strcmp(name,"多云"))
    {
        return DAYU;
    }
    if(strstr(name,"阵雨"))
    {
        return ZHENYU;
    }
    if(!strcmp(name,"雨"))
    {
        return XIAOYU;
    }
    if(!strcmp(name,"阴"))
    {
        return YIN;
    }
    if(!strcmp(name,"暴雨"))
    {
        return BAOYU;
    }

}


void show_weather_pic(int n,int x)
{

   
    if(n == QING)
    {
        /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */
        lcd_draw_jpg(x,350,"/zhou1/project3/pic/qing.jpg");
        /* 关闭LCD设备 */
        lcd_close();
    }
    if(n == DAYU)
    {
          /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */
        lcd_draw_jpg(x,350,"/zhou1/project3/pic/duoyun.jpg");
        /* 关闭LCD设备 */
        lcd_close();

    }
    if(n == XIAOYU)
    {
        /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */
        lcd_draw_jpg(x,350,"/zhou1/project3/pic/yu.jpg");
        /* 关闭LCD设备 */
        lcd_close();

    }
 
    if(n == BAOYU)
    {
        /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */
        lcd_draw_jpg(x,350,"/zhou1/project3/pic/baoyu.jpg");
        /* 关闭LCD设备 */
        lcd_close();
    }
    if(n == ZHENYU)
    {
        /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */
        lcd_draw_jpg(x,350,"/zhou1/project3/pic/zhneyu.jpg");
        /* 关闭LCD设备 */
        lcd_close();
    }   
    if(n == YIN)
    {    

        /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */
        lcd_draw_jpg(x,350,"/zhou1/project3/pic/111.jpg");
        /* 关闭LCD设备 */
        lcd_close();
    }
            
       

}
int main()
{
    
    pthread_create(&tid,NULL,picture,NULL);
    pthread_detach(tid);
    while(1)
    {
        printf("请输入需要的功能！\n");
        char cheak[20];
        scanf("%s",cheak);
        chark_tmd(cheak);
        switch (flag)
        {
        case 1:
            weather();
            flag = 0;
            break;
        
        case 2:
            chat();
            flag = 0;
            break;
        
        case 3:
            sleep(5);
            pthread_create(&tid,NULL,picture,NULL);
            pthread_detach(tid);
            flag =0;
            break;
        case 4:
            picture_download();
            flag = 0;
            break;
        case 5:
            pthread_cancel(tid);
            sleep(1);
            Shut_down();
            exit(0);
            flag = 0;
        case 6:
            famous_sayings();
            flag = 0;
            break;
        case 7:
            translate();
            flag = 0;
            break;
        }

    
            
    }
    
    while(1);
    return 5;
}


int Shut_down()
{

     for(int i=1;i<=35;i++)
        {
        char tmd[50];
        bzero(tmd,50);
        sprintf(tmd,"/zhou1/project3/pic2/1-%d.jpg",i);
        /* 打开LCD设备 */
	    lcd_open();
	    /* 在x=0,y=0,显示图片 */
	    lcd_draw_jpg(0,0,tmd);
	    /* 关闭LCD设备 */
	    lcd_close();
        
        
        }
    return 0;
}


int chark_tmd(char * name)
{

        if(strstr(name,"天气"))
        {
            flag = 1;
        }
        if(strstr(name,"聊天"))
        {
            flag = 2;
        }
        if(strstr(name,"退出"))
        {
            flag = 3;
        }
        if(strstr(name,"下载图片"))
        {
            flag = 4;
        }
        if(strstr(name,"关机"))
        {
            flag = 5;
        }
        if(strstr(name,"名人名言"))
        {
            flag = 6;
        }
         if(strstr(name,"翻译"))
        {
            flag = 7;
        }
        
    
    return 8;

}


int translate()
{
      a:
    {
        int tcp_socket = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(80);
        //server_addr.sin_addr.s_addr = inet_addr("110.43.34.66");
        server_addr.sin_addr.s_addr = inet_addr("47.107.155.132");

   
        int ret = connect(tcp_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
         if (ret < 0)
         {
             perror("请求失败！");
         }
         else
         {
             printf("链接成功！\n");
         }
        //char *http_head = "GET /bangumi/play/ep341100 HTTP/1.1\r\nHost:www.bilibili.com\r\n\r\n";
          
                printf("请输入要翻译的文稿 格式：（翻译 xxxx）:\n");
                char buf[1024]= {0};
                scanf("%s",buf);
                if(!strcmp(buf,"exit"))
                {
                    show_chat("正在退出翻译！");
                    sleep(3);
                    show_chat("需要什么帮助呢？");
                    return 0;
                }
                show_chat("正在为您翻译！请稍后.....");
                sleep(1);
                char http_head[1024] = {0};
                sprintf(http_head,"GET /api.php?key=free&appid=0&msg=%s HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n",buf);
                
                
                write(tcp_socket,http_head,strlen(http_head));
                
                char rbuf[1024] = {0};
                char data[100] = {0};
                char data_t[100] = {0};
                read(tcp_socket,rbuf,1024);
                if(strlen(rbuf)==0)
                {
                    show_chat("刚才开了个小差- -请重新输入！");
                    goto a;
                }
                char * p= strstr(rbuf,"\"result\":0,\"content\":\"");
                p = p+strlen("\"result\":0,\"content\":\"");
                char * q = strstr(p,"\"");
                int len = (int)(q - p);
                strncpy(data,p,len);

                char * p_t = strstr(data,"\"{br}");
                p_t = p_t+strlen("\"{br}");
                char * p_f = strstr(p_t,"}");
                int len_t =(int)(p_t-p_f);
                strncpy(data_t,p_t,len_t);


                printf("data %s\n",data_t);
                // tmp2 = strtok(data,"\"{br}");
                // while(1)
                // {
                //     tmp2 = strtok(NULL,"\"{br}");
                //     if (tmp2 == NULL)
                //     {
                //             break;
                //     }
                //     if (tmp2[0] == '0')
                //     {
                //         continue;
                //     }
                // }
                show_chat(data_t);

                sleep(10);
                show_chat("需要什么帮助呢？");
                // pthread_create(&tid,NULL,picture,NULL);
                // pthread_detach(tid);
        }
     
        
        
        
        return 0;

}

int famous_sayings()
{

        // pthread_cancel(tid);
        int tcp_socket2 = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(80);
        //server_addr.sin_addr.s_addr = inet_addr("110.43.34.66");
        server_addr.sin_addr.s_addr = inet_addr("113.96.178.42");
        int ret = connect(tcp_socket2,(struct sockaddr*)&server_addr,sizeof(server_addr));
            if (ret < 0)
            {
                perror("请求失败！");
            }
            else
            {
                printf("链接成功！\n");
            }
            char tmd[] = "GET /api/mingyan HTTP/1.1\r\nHost:v1.alapi.cn\r\n\r\n";

            write(tcp_socket2,tmd,strlen(tmd));
            char buf[100];
            while(1)
            {
                read(tcp_socket2,buf,100);
                        
                        p2= strstr(buf,"{\"code\":");
                            if(p2 !=NULL)
                            {
                                break;
                            }
            
            }       
        cJSON *obj=cJSON_Parse(p2);
			if(obj == NULL)
			{
				printf("不是JSO数据\n");
				return 0;
			}
            
        cJSON *value_date=cJSON_GetObjectItem(obj,"date");
        cJSON *value_content = cJSON_GetObjectItem(value_date,"content");
        cJSON *value_author = cJSON_GetObjectItem(value_date,"author");



        printf("content= %s\n",value_content->valuestring);
        printf("autho = %s\n",value_author->valuestring);


        char tmp[100];
        sprintf(tmp,"%s\n\n %s",value_content->valuestring,value_author->valuestring);
        

        pthread_cancel(tid);
         /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */

        lcd_draw_jpg(0,0,"/zhou1/project3/111.jpg");
    
        /* 关闭LCD设备 */
        lcd_close();
            
        show_weather(tmp,0,0,800,480);    
        
        // int size;
        // size = stelen(tmp);
        // if(size > 24);
        // {
            
        // }
        sleep(10);
        pthread_create(&tid,NULL,picture,NULL);
        pthread_detach(tid);
        return 0;
}






void *picture()
{
   
     /* 打开LCD设备 */
    lcd_open();
    /* 在x=0,y=0,显示图片 */

    lcd_draw_jpg(0,0,"/zhou1/project3/111.jpg");
   
     /* 关闭LCD设备 */
	lcd_close();
    show_chat("需要什么帮助呢？");
    char tmd[50];
    while(1)
    {
        for(int i=1;i<=35;i++)
        {
        bzero(tmd,50);
        sprintf(tmd,"/zhou1/project3/pic/1-%d.jpg",i);
        /* 打开LCD设备 */
	    lcd_open();
	    /* 在x=0,y=0,显示图片 */
	    lcd_draw_jpg(0,0,tmd);
	    /* 关闭LCD设备 */
	    lcd_close();
        if(i==35)
        {
            i=1;
        }
        
        }
    }
   
}


int chat()
{
    a:
    {
        int tcp_socket = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(80);
        //server_addr.sin_addr.s_addr = inet_addr("110.43.34.66");
        server_addr.sin_addr.s_addr = inet_addr("47.107.155.132");

   
        int ret = connect(tcp_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
         if (ret < 0)
         {
             perror("请求失败！");
         }
         else
         {
             printf("链接成功！\n");
         }
        //char *http_head = "GET /bangumi/play/ep341100 HTTP/1.1\r\nHost:www.bilibili.com\r\n\r\n";
            while(1)
            {
                printf("请输入问题:\n");
                char buf[1024]= {0};
                scanf("%s",buf);
                if(!strcmp(buf,"exit"))
                {
                    show_chat("正在退出聊天！");
                    sleep(3);
                    show_chat("需要什么帮助呢？");
                    return 0;
                }
                char http_head[1024] = {0};
                sprintf(http_head,"GET /api.php?key=free&appid=0&msg=%s HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n",buf);
                
                
                write(tcp_socket,http_head,strlen(http_head));
                
                char rbuf[1024] = {0};
                char data[100] = {0};
                read(tcp_socket,rbuf,1024);
                if(strlen(rbuf)==0)
                {
                    show_chat("刚才开了个小差- -请重新输入！");
                    goto a;
                }
                char * p= strstr(rbuf,"\"result\":0,\"content\":\"");
                p = p+strlen("\"result\":0,\"content\":\"");
                char * q = strstr(p,"\"");
                int len = (int)(q - p);
                strncpy(data,p,len);
                printf("%s\n",data);
                show_chat(data);
            }        
    }
    
        return 0;
}


struct LcdDevice *init_lcd(const char *device)
{
	//申请空间
	struct LcdDevice* lcd = malloc(sizeof(struct LcdDevice));
	if(lcd == NULL)
	{
		return NULL;
	} 

	//1打开设备
	lcd->fd = open(device, O_RDWR);
	if(lcd->fd < 0)
	{
		perror("open lcd fail");
		free(lcd);
		return NULL;
	}
	
	//映射
	lcd->mp = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd->fd,0);

	return lcd;
}

void show_chat(char * name)
{

//初始化Lcd
        struct LcdDevice* lcd = init_lcd("/dev/fb0");
                
        //打开字体	
        font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
        
        //字体大小的设置
        fontSetSize(f,40);
        
       
        //创建一个画板（点阵图）
        bitmap *bm = createBitmapWithInit(480,100,4,getColor(0,49,31,14)); //也可使用createBitmapWithInit函数，改变画板颜色
        //bitmap *bm = createBitmap(288, 100, 4);
        
        // char buf[] = "Hello 未来";
        
        //将字体写到点阵图上		  //   b  g  r   
        fontPrint(f,bm,0,0,name,getColor(0,255,255,0),0);
                
        //getColor(0,B,G,R)

        //把字体框输出到LCD屏幕上
        show_font_to_lcd(lcd->mp,0,380,bm);

        //把字体框输出到LCD屏幕上
        //show_font_to_lcd(lcd->mp,200,200,bm);

        //关闭字体，关闭画板
        fontUnload(f);
        destroyBitmap(bm);
       

}

void show_weather(char * name,int x,int y,int chang,int gao)
{
    //初始化Lcd
        struct LcdDevice* lcd = init_lcd("/dev/fb0");
                
        //打开字体	
        font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
        
        //字体大小的设置
        fontSetSize(f,30);
        
       
        //创建一个画板（点阵图）
        bitmap *bm = createBitmapWithInit(chang,gao,4,getColor(0,49,31,14)); //也可使用createBitmapWithInit函数，改变画板颜色
        //bitmap *bm = createBitmap(288, 100, 4);
        
        // char buf[] = "Hello 未来";
        
        //将字体写到点阵图上		  //   b  g  r   
        fontPrint(f,bm,0,0,name,getColor(0,255,255,0),0);
                
        //getColor(0,B,G,R)

        //把字体框输出到LCD屏幕上
        show_font_to_lcd(lcd->mp,x,y,bm);

        //把字体框输出到LCD屏幕上
        //show_font_to_lcd(lcd->mp,200,200,bm);

        //关闭字体，关闭画板
        fontUnload(f);
        destroyBitmap(bm);
}


// void cheak()
// {

//     int x,y;
//     int fd_ts = chuno_init( );
//     while(1)
//     {
//         ts_get(&x, &y , fd_ts  );
//         printf("(%d,%d)\n" ,x , y ); 
//         if((x>704&&x<777)&&(y>434&&y<470))
//         {
//             flag = 1;
//         }
//     }
// }

int  weather()
{
   
   
   
        // pthread_cancel(tid);
        int tcp_socket1 = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(80);
        //server_addr.sin_addr.s_addr = inet_addr("110.43.34.66");
        server_addr.sin_addr.s_addr = inet_addr("1.81.5.190");
        int ret = connect(tcp_socket1,(struct sockaddr*)&server_addr,sizeof(server_addr));
            if (ret < 0)
            {
                perror("请求失败！");
            }
            else
            {
                printf("链接成功！\n");
            }
            //char *http_head = "GET /bangumi/play/ep341100 HTTP/1.1\r\nHost:www.bilibili.com\r\n\r\n";
            while (1)
            {
                printf("请输入城市名:\n");
                char buf[4096]= {0};
                scanf("%s",buf);
                if (!strcmp(buf,"退出"))
                {
                    return 8;
                }
                show_chat("正在查询天气！");
                city  c;
                find_city_id(buf,&c);
                char http_head[4096] = {0};
                sprintf(http_head,"GET /api/weather/city/%s HTTP/1.1\r\nHost:t.weather.itboy.net\r\n\r\n",c.id);
                
                
                write(tcp_socket1,http_head,strlen(http_head));
                while(1)
                {
                    read(tcp_socket1,rbuf1,4096*10);
                    p1= strstr(rbuf1,"{\"message\":");
                        if(p1 !=NULL)
                        {
                            break;
                        }
                       
                }     
                break;
            }
    
    //1.判断是否为 json 数据  
	cJSON *obj=cJSON_Parse(p1);
			if(obj == NULL)
			{
				printf("不是JSO数据\n");
				return 0;
			}
    cJSON *value_date=cJSON_GetObjectItem(obj,"date");
    cJSON *value_c=cJSON_GetObjectItem(obj,"cityInfo");
    cJSON *value_city=cJSON_GetObjectItem(value_c,"city");
    cJSON *value_parent=cJSON_GetObjectItem(value_c,"parent");
    cJSON *value_citykey=cJSON_GetObjectItem(value_c,"citykey");
    sig(tid);//信号处理函数
    usleep(200);
    /* 打开LCD设备 */
    lcd_open();
    /* 在x=0,y=0,显示图片 */
    lcd_draw_jpg(0,0,"/zhou1/project3/111.jpg");
     /* 关闭LCD设备 */
	lcd_close();
    printf("date = %s\n",value_date->valuestring);
    printf("city = %s\n",value_city->valuestring);
    printf("parent = %s\n",value_parent->valuestring);
    printf("citykey = %s\n",value_citykey->valuestring);


    char nam[50];
    sprintf(nam,"%s  %s %s ",
    value_date->valuestring,value_city->valuestring,value_parent->valuestring,value_citykey->valuestring);

    show_weather(nam,0,0,800,40);
    cJSON *value_data=cJSON_GetObjectItem(obj,"data");
    //cJSON *value_forecast=cJSON_GetObjectItem(value_data,"forecast");
    cJSON *arry=cJSON_GetObjectItem(value_data,"forecast");
            if (arry == NULL)
            {
                printf("数组取出失败\n");
                return 0;
            }
        
        //求出数组大小
        int size = cJSON_GetArraySize(arry);
        printf("当前数组的大小%d\n",size);
        int sum = 0;    
        for (int i = 0; i < size; i++)
        {
            cJSON * new_obj = cJSON_GetArrayItem(arry,i);
            if (new_obj == NULL)
            {
                printf("获取对象失败\n");
                return 0;
            }
           
            cJSON *value1=cJSON_GetObjectItem(new_obj,"high");
            cJSON *value2=cJSON_GetObjectItem(new_obj,"low");
            cJSON *value3=cJSON_GetObjectItem(new_obj,"ymd");
            cJSON *value4=cJSON_GetObjectItem(new_obj,"week");
            cJSON *value5=cJSON_GetObjectItem(new_obj,"fx");    
            cJSON *value6=cJSON_GetObjectItem(new_obj,"fl");    
            cJSON *value7=cJSON_GetObjectItem(new_obj,"type");    
            cJSON *value8=cJSON_GetObjectItem(new_obj,"notice");    
            
            printf("第 %d 天\n",i);
            printf("高温 %s\n",value1->valuestring);
            printf("低温 %s\n",value2->valuestring);
            printf("日期 %s\n",value3->valuestring);
            printf("星期 %s\n",value4->valuestring);
            printf("风向 %s\n",value5->valuestring);
            printf("风力 %s\n",value6->valuestring);
            printf("天气 %s\n",value7->valuestring);
    
            
            char nam1[50];
            sprintf(nam1," 未来 %d 天\n  %s\n  %s\n\n  %s\n  %s\n  %s\n  %s\n  %s\n",i,
                    value1->valuestring,value2->valuestring,value3->valuestring,value4->valuestring,
                    value5->valuestring,value6->valuestring,value7->valuestring);
                    
            show_weather(nam1,sum,50,133,260);
            int ret1 = cheak_weather(value7->valuestring);
            show_weather_pic(ret1,sum);
            sum = sum + 133;
            if(i == 5)
                {
                    break;
                }
    
        }   
           
}

/*  
    函数名：
            bool find_city_id(char *cmd,city *c)
    参数：
            char cmd : 城市名称
            city *c:   城市结构体（方便读取的，可以只用城市名称对比，返回id值）
    返回值：
            成功        返回true
            失败        返回false
    功能：  
        从文件中把数据读出来，并比对城市名称，然后将对应的id存入结构体中，以便使用

    注意：文件需要UTF-8编码格式的，直接的txt文件并不能直接使用


*/

bool find_city_id(char *cmd, city *c)
{

    FILE *fp=fopen("./city.txt","r");//打开文件
    if(fp==NULL)
    {
        printf("文件打开错误！\n");

        sleep(5);

        return false;
    }
    while(1)
    {
        bzero(c->name,50);
        bzero(c->id,20);
        
  
        fscanf(fp,"%s %s\n",c->id,c->name);  //格式化读取文件内容并存到对应的结构体变量中
        if(strstr(cmd,c->name))         //比较是否为要查询的城市
        {  
            fclose(fp);
            return true;
        }

        if(feof(fp))        //读取到末尾失败返回
        {
            fclose(fp); 
            return false;
        }

    }


}

void sig(int arg)//信号处理函数
{
	
          pthread_cancel(tid);
		
    
}


int picture_download()
{
  
  
    struct hostent *host =  gethostbyname("www.dmoe.cc");
  
    int tcp_socket1 = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in service_addr_1;
    service_addr_1.sin_port = htons(80);
    service_addr_1.sin_family = AF_INET;
    service_addr_1.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));

    int ret = connect(tcp_socket1, (struct sockaddr *)&service_addr_1,sizeof(service_addr_1));
        if (ret == -1 )
        {
            printf("connect lost!!!\n");
            return -1;
        }else
        {
            printf("connect success!!!\n");
        }

        

        char *http_head_1 = "GET /random.php HTTP/1.1\r\nHOST:www.dmoe.cc\r\n\r\n";

        write(tcp_socket1,http_head_1,strlen(http_head_1));

        char  head_1[2048*100] ={0};
        read(tcp_socket1,head_1,sizeof(head_1));
        printf("%s\n",head_1);

		char *p = strstr(head_1,"Location: ");
        char *p1 = strstr(head_1,".jpg");
        int txt_len = (int)(p1-p);
        char content[1024] = {0};
        strncpy(content,p,txt_len);
        
        char *ch ="/";
        char *p3 = strtok(content,ch);
        char new_http_head[1024] = {0};
        char yuming[50] ={0};
        char URL[100] ={0};
        
        p3 = strtok(NULL,ch);
        
        strcpy(yuming,p3);
        
        p3 = strtok(NULL,ch);
        
        sprintf(URL,"/%s",p3);
        p3 = strtok(NULL,ch);
        
        sprintf(URL,"%s/%s.jpg",URL,p3);
        

        sprintf(new_http_head,"GET %s HTTP/1.1\r\nHOST:%s\r\n\r\n",URL,yuming);
        struct hostent*main_host =  gethostbyname(yuming);

        printf("%s\n",new_http_head);


#ifdef DEBUG	
   //1.创建TCP socket  
       
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

  // 2.设置服务器的IP地址并链接    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); //HTTP 协议默认的端口就是 80 
    server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)main_host->h_addr_list[0])); //服务器的IP地址
											//图片的地址 
    

  // 3.链接 
  int ret1 = connect(tcp_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
	  if(ret1 < 0)
	  {
		printf("链接失败\n");
		return 0;
	  }
	  else
	  {
		  printf("链接成功\n");
	  }

	//发HTTTP 请求协议  
	write(tcp_socket,new_http_head,strlen(new_http_head));
	
	
	//新建一个文件  


       int fd=open("1.jpeg",O_RDWR|O_CREAT|O_TRUNC,0777);
			if(fd < 0)
			{
				perror("");
				return -1;
			}
	
	//读取头数据  
	char  head[2048]={0}; 
	int size1 = read(tcp_socket,head,2048);  
		printf("size1 = %d\n",size1);
	//取出文件的大小 
	int file_size=0;
	//下载的大小 
	int load_size=0;
	
	char *p10 =  strstr(head,"Content-Length");  //查找关键字
	
	sscanf(p10,"Content-Length:%d\r\n",&file_size);
	
	printf("file_size=%d\n",file_size);
	
	
	//求出头数据的长度 
	p10 = strstr(head,"\r\n\r\n");   //回文末尾  
	p10 = p10+4; 
	
  int head_len = (int)(p10 - head);
      printf("head_len = %d\n",head_len);
	
	//写入去头的一次数据 
	  int len  =size1-head_len; 
	  write(fd,p10,len);
	  load_size =+  len;
	
	
	while(1)
	{
	//回收HTTP 服务器的消息
	char  buf[4096]={0};
	int size=read(tcp_socket,buf,4096);
		load_size += size;
		printf("file_size=%d load_size=%d\n",file_size,load_size);
        if(file_size == load_size)
		{
			printf("图片下载完毕\n");
			write(fd,buf,size);
			break;
		}
	     //把数据写入到本地文件中 
		   write(fd,buf,size);
	
		//printf("buf=%s\n",buf);
    
	}
        pthread_cancel(tid);
    /* 打开LCD设备 */
        lcd_open();
        /* 在x=0,y=0,显示图片 */
        lcd_draw_jpg2(0,0,"/zhou1/project3/1.jpeg",1);
        /* 关闭LCD设备 */
	    lcd_close();
        sleep(10);
        pthread_create(&tid,NULL,picture,NULL);
        pthread_detach(tid);
        return 0;
#endif	
	/* close(fd); 
	close(tcp_socket); */
}