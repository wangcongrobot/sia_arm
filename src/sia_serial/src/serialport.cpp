//serial_port.cpp
#include <serial/serial.h>
#include <iostream>
#include <data_record.h>

void SD_msg(uint8_t* sd_msg, int n);
void RX_msg(uint8_t* rx_msg, size_t n);
//创建一个serial类
serial::Serial sp;

int flag_SD = 0;//0代表没有发送 1代表正在发送
int flag_RX = 0;//0代表没有接受 1代表正在接受

uint8_t rx_buffer[24];
uint8_t sd_buffer[24] = {0x4d, 0x01, 0xa0, 0x07, 0x24, 0x0c, 0x60, 0x03, 0x90, 0x09, 0x60, 0x03, 0x5e, 
                         0x01, 0x00, 0x04, 0x14, 0x00, 0x00, 0x2a, 0x13, 0x2c, 0x0d, 0x0a};

int main(int argc, char** argv)
{
    
    ros::init(argc, argv, "serial_port");
    //创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
    ros::NodeHandle n;
    

    //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyUSB0");
    //设置串口通信的波特率
    sp.setBaudrate(19200);
    //串口设置timeout
    sp.setTimeout(to);
    

    try
    {
        //打开串口
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    //判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {
        return -1;
    }
    SD_msg(sd_buffer,24);
    SD_dataRecord(sd_buffer,6,'0','5','0');
    ros::Rate loop_rate(50);
    while(ros::ok())
    {
        //获取缓冲区内的字节数
        size_t n = sp.available();
        if((n!=0)&&(flag_SD == 0))
        {
            RX_msg(rx_buffer,  n);
            flag_SD = 0;
        }
        
        loop_rate.sleep();
    }
    
    //关闭串口
    sp.close();
 
}


void SD_msg(uint8_t* sd_msg, int n)
{
    if(flag_RX == 1)
    {
        ROS_INFO_STREAM("serial is receiving data");
    }
    else
    {
        flag_SD = 1;   
        sp.write(sd_msg, n);
        flag_SD = 0;
    }
    
}

void RX_msg(uint8_t* rx_msg, size_t n)
{
        flag_RX = 1;       
        if(n!=0)
        {
            //uint8_t buffer[1024];
            //校验并读出数据
            n = sp.read(rx_buffer, n);
            if(rx_buffer[0] == 0x4d)
            {
                for(int i=0; i<n; i++)//16进制的方式打印到屏幕
                {
                    if( (rx_buffer[i] & 0xff) > 0x0f)
                    {
                        std::cout << "0x" << std::hex << (rx_buffer[i] & 0xff) << " ";
                    }
                    else
                    {
                        std::cout << "0x0" << std::hex << (rx_buffer[i] & 0xff) << " ";
                    }
                }
                std::cout << std::endl;
                ROS_INFO_STREAM("confirm successfully");
            }
            else
            {
                for(int i=0; i<n; i++)//16进制的方式打印到屏幕
                {
                    if( (rx_buffer[i] & 0xff) > 0x0f)
                    {
                        std::cout << "0x" << std::hex << (rx_buffer[i] & 0xff) << " ";
                    }
                    else
                    {
                        std::cout << "0x0" << std::hex << (rx_buffer[i] & 0xff) << " ";
                    }
                }
                std::cout << std::endl;
                ROS_INFO_STREAM("confirm failed.");
            }
            
        }
}



