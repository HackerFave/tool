#ifndef COMMON
#define COMMON

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <QDateTime>
#include <QDebug>

#if defined(__linux)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <net/if.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include "hal-api.h"
#endif

#define MCGS_COMPONENT_API_INVOKE_SUCCESS 0

#define ENABLE_SINGLETON_RESTRAINT(T) \
    private: \
        T(); \
        T(const T&); \
        const T& operator = (const T&);

/**
 * @brief The linux_sys_info_c class
 */
class platform_sys_info_c
{
public:
    /**
     * @brief get_memory_info
     * @param system_available_memory
     * @param system_physical_memory
     * @retval
     */
    static int get_memory_info(int& system_available_memory, int& system_physical_memory)
    {
        FILE* fp = NULL;

        system_available_memory = -1;
        system_physical_memory = -1;
        /*read original information*/
#if defined(_WINDOWS_) || defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
        errno_t err = fopen_s(&fp, "/proc/meminfo", "r");
#else
        fp = fopen("/proc/meminfo", "r");
#endif

#if defined(_WINDOWS_) || defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
        if(err != 0)
#else
        if(fp == NULL)
#endif

        {
            return -1;
        }
        else
        {
            char line[1024] = {0};
            int system_memfree = -1;
            int system_buffers = -1;
            int system_cached = -1;

            while(system_physical_memory == -1 || system_memfree == -1
                  || system_buffers == -1 || system_cached == -1 )
            {
                char sep[] = " ";
                char* token = NULL;
                char* saveptr = NULL;

                if(!fgets(line, 1024, fp))
                {
                    fclose(fp);
                    return -1;
                }

#if defined(_WINDOWS_) || defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
                token = strtok_s(line, sep, &saveptr);
#else
                token = strtok_r(line, sep, &saveptr);
#endif
                if(token && strstr(token, "MemTotal"))
                {
#if defined(_WINDOWS_) || defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
                    token = strtok_s(NULL, sep, &saveptr);
#else
                    token = strtok_r(NULL, sep, &saveptr);
#endif
                    if(token)
                    {
                        system_physical_memory = atoi(token);
                    }
                    else
                    {
                        fclose(fp);
                        return -1;
                    }
                }
                else if(token && strstr(token, "MemFree"))
                {
#if defined(_WINDOWS_) || defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
                    token = strtok_s(NULL, sep, &saveptr);
#else
                    token = strtok_r(NULL, sep, &saveptr);
#endif
                    if(token)
                    {
                        system_memfree = atoi(token);
                        system_available_memory += system_memfree;
                    }
                    else
                    {
                        fclose(fp);
                        return -1;
                    }
                }
                else if(token && strstr(token, "Buffers"))
                {
#if defined(_WINDOWS_) || defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
                    token = strtok_s(NULL, sep, &saveptr);
#else
                    token = strtok_r(NULL, sep, &saveptr);
#endif
                    if(token)
                    {
                        system_buffers = atoi(token);
                        system_available_memory += system_buffers;
                    }
                    else
                    {
                        fclose(fp);
                        return -1;
                    }
                }
                else if(token && strstr(token, "Cached"))
                {
#if defined(_WINDOWS_) || defined(WIN32) || defined(_WIN32) || defined(WIN64)|| defined(_WIN64)
                    token = strtok_s(NULL, sep, &saveptr);
#else
                    token = strtok_r(NULL, sep, &saveptr);
#endif
                    if(token)
                    {
                        system_cached = atoi(token);
                        system_available_memory += system_cached;
                    }
                    else
                    {
                        fclose(fp);
                        return -1;
                    }
                }
            }
            fclose(fp);
        }
        return 0;
    }

#if defined(__linux)
    /**
     * @brief get_disk_info from root directory
     * @param system_available_disk
     * @param system_total_disk
     * @return
     */
    static int get_disk_info(unsigned long long& system_available_disk, unsigned long long& system_total_disk)
    {
        struct statfs stat;

        if(!statfs("/", &stat))
        {
            unsigned long long blocksize = stat.f_frsize;
            system_total_disk = stat.f_blocks * blocksize;
            system_available_disk = stat.f_bavail * blocksize;
            return 0;
        }
        else
        {
            return -1;
        }
    }


    /**
     * @brief linux_sys_info_c::get_disk_info
     * @param directory
     * @param system_available_disk
     * @param system_total_disk
     * @return
     */
    static int get_disk_info(const char* directory, unsigned long long& system_available_disk,
                             unsigned long long& system_total_disk)
    {
        struct statfs stat;

        if(!statfs(directory, &stat))
        {
            system_total_disk = stat.f_blocks * stat.f_frsize;
            system_available_disk = stat.f_bavail * stat.f_frsize;
            return 0;
        }
        else
        {
            return -1;
        }
    }

    static int get_mac(unsigned char* mac_addr)
    {
        int sock_mac;
        struct ifreq ifr_mac;

        sock_mac = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_mac == -1)
        {
            printf("getmac socket invoke failed: %s\n", strerror(errno));
            return -1;
        }

        memset(&ifr_mac, 0, sizeof(ifr_mac));
        strncpy(ifr_mac.ifr_name, "eth0", sizeof(ifr_mac.ifr_name) - 1);

        if((ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
        {
            printf("getmac ioctl invoke failed: %s\n", strerror(errno));
            close(sock_mac);
            return -1;
        }

        mac_addr[0] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0];
        mac_addr[1] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1];
        mac_addr[2] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2];
        mac_addr[3] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3];
        mac_addr[4] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4];
        mac_addr[5] = (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5];

        close(sock_mac);

        return 0;
    }

    static QDateTime get_rtc_time()
    {
//        QDateTime dateTime;
//        struct rtc_time rtc_tm;
//        int fd = -1;
//        if(-1 == (fd = open("/dev/rtc", O_RDONLY)))
//        {
//            printf("open /dev/rtc failed: %s!\n", strerror(errno));
//            return dateTime;
//        }

//        /* Read the RTC time/date */
//        if(ioctl(fd, RTC_RD_TIME, &rtc_tm) == -1)
//        {
//           printf("get_rtc_time ioctl invoke failed: %s\n", strerror(errno));
//           close(fd);
//           return dateTime;
//        }

//        dateTime.setDate(QDate(rtc_tm.tm_year + 1900, rtc_tm.tm_mon + 1, rtc_tm.tm_mday));
//        dateTime.setTime(QTime(rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec));

//        close(fd);
//        return dateTime;

        QDateTime dateTime;
        struct tm rtc_tm;

        int retval = hal_get_rtc_time(&rtc_tm);
        if(retval != 0)
        {
            qDebug("get_rtc_time failed: %d!", retval);
        }

        dateTime.setDate(QDate(rtc_tm.tm_year + 1900, rtc_tm.tm_mon + 1, rtc_tm.tm_mday));
        dateTime.setTime(QTime(rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec));

        return dateTime;
    }

    static bool set_rtc_time(QDateTime& dateTime)
    {
//        int fd = -1;
//        if(-1 == (fd = open("/dev/rtc", O_WRONLY)))
//        {
//            printf("open /dev/rtc failed: %s!\n", strerror(errno));
//            return false;
//        }

//        struct rtc_time rtcTime;
//        rtcTime.tm_year = dateTime.date().year() - 1900;
//        rtcTime.tm_mon = dateTime.date().month() - 1;
//        rtcTime.tm_mday = dateTime.date().day();
//        rtcTime.tm_hour = dateTime.time().hour();
//        rtcTime.tm_min = dateTime.time().minute();
//        rtcTime.tm_sec = dateTime.time().second();
//        rtcTime.tm_isdst = 0;
//        rtcTime.tm_yday = dateTime.date().dayOfYear() - 1;
//        rtcTime.tm_wday = dateTime.date().dayOfWeek() - 1;


//        if(-1 == ioctl(fd, RTC_SET_TIME, &rtcTime))
//        {
//            printf("ioctl set rtc time failed: %s!\n", strerror(errno));
//            close(fd);
//            return false;
//        }

//        close(fd);
//        return true;

        struct tm rtcTime;
        rtcTime.tm_year = dateTime.date().year() - 1900;
        rtcTime.tm_mon = dateTime.date().month() - 1;
        rtcTime.tm_mday = dateTime.date().day();
        rtcTime.tm_hour = dateTime.time().hour();
        rtcTime.tm_min = dateTime.time().minute();
        rtcTime.tm_sec = dateTime.time().second();
        rtcTime.tm_isdst = 0;
        rtcTime.tm_yday = dateTime.date().dayOfYear() - 1;
        rtcTime.tm_wday = dateTime.date().dayOfWeek() - 1;

        int retval = hal_set_rtc_time(&rtcTime);
        if(retval != 0)
        {
            qDebug("set_rtc_time failed: %d!", retval);

            return false;
        }

        return true;
    }

    static bool rtc_check()
    {
//        #define RTC_BUF_SIZE 20

//        bool ret = false;
//        char sbuf[RTC_BUF_SIZE + 1] = {0};
//        int fd = open("/sys/class/rtc/rtc0/low", O_RDONLY);

//        if(fd < 0)
//        {
//            printf("/sys/class/rtc/rtc0/low open failed: %s!\n", strerror(errno));
//            return ret;
//        }

//        int rc = 0;
//        if((rc = read(fd, sbuf, RTC_BUF_SIZE)) > 0)
//        {
//            char dest_s[] = "NORMAL";
//            if(0 == memcmp(sbuf, dest_s, 6))
//            {
//                ret = true;
//            }
//        }
//        else
//        {
//            printf("rtc_check read failed: %s, read count is: %d!\n", strerror(errno), rc);
//        }

//        close(fd);
//        return ret;

        int retval = hal_get_rtc_status();

        if(retval != 0)
        {
            qDebug("get rtc status failed: %d!", retval);
            return false;
        }

        return true;
    }

    static bool set_usb_mode(const char* val, int length)
    {
        int fd = open("/sys/devices/platform/sunxi_usb_udc/otg_role", O_WRONLY);
        if(fd < 0)
        {
            printf("/sys/devices/platform/sunxi_usb_udc/otg_role open failed: %s!\n", strerror(errno));
            return false;
        }

        int wc = write(fd, val, length);
        if(wc <= 0)
        {
            printf("set_usb_mode write failed: %s, write count is: %d!\n", strerror(errno), wc);
            close(fd);
            return false;
        }

        close(fd);
        return true;
    }

    static bool set_usb_master_mode()
    {
        int retval = hal_set_usb_mode(HAL_USB_OTG_MASTER);

        if(retval != 0)
        {
            qDebug("set usb mode to master failed: %d!", retval);

            return false;
        }

        return true;
    }

    static bool set_usb_slave_mode()
    {
        int retval = hal_set_usb_mode(HAL_USB_OTG_DEVICE);

        if(retval != 0)
        {
            qDebug("set usb mode to slave failed: %d!", retval);

            return false;
        }

        return true;
    }

    static int get_usb_mode()
    {
//        #define USB_BUF_SIZE 10

//        char buf[USB_BUF_SIZE + 1] = {0};
//        int fd = open("/sys/devices/platform/sunxi_usb_udc/otg_role", O_RDONLY);
//        if(fd < 0)
//        {
//            printf("/sys/devices/platform/sunxi_usb_udc/otg_role open failed: %s!\n", strerror(errno));
//            return -errno;
//        }

//        int rc = 0;
//        if((rc = read(fd, buf, USB_BUF_SIZE)) > 0)
//        {
//            close(fd);
//            return atoi(buf);
//        }
//        else
//        {
//            printf("get_usb_mode read failed: %s, read count is: %d!\n", strerror(errno), rc);
//            close(fd);
//            return -errno;
//        }

        return hal_get_usb_mode();
    }

    static bool set_sys_ip_addr(const QString &ipStr,
                           const QString &netMaskStr,
                           const QString &gatewayStr)
    {
        int retval = 0;
        struct hal_network net_info;
        retval = hal_get_network(0, &net_info);
        if(retval != 0)
        {
            qDebug("CSEVR get network information failed when set set network attribute:%d!", retval);
        }

        struct hal_ip_config conf;
        struct in_addr iaddr;
        inet_aton(ipStr.toStdString().c_str(), &iaddr);
        conf.ipv4.address = iaddr.s_addr;

        struct in_addr imask;
        inet_aton(netMaskStr.toStdString().c_str(), &imask);
        conf.ipv4.netmask = imask.s_addr;

        struct in_addr igate;
        inet_aton(gatewayStr.toStdString().c_str(), &igate);
        conf.ipv4.gw = igate.s_addr;

        conf.ipv4.addr_type = HAL_ADDR_TYPE_STATIC;

        retval = hal_set_ip_addr(net_info.name, &conf);
        if(retval != 0)
        {
            qDebug("CSEVR set network information failed: %d!", retval);
            return false;
        }
        else
        {
            return true;
        }
    }

    static bool get_sys_ip_addr(QString &ip,
                                QString &mask,
                                QString &gateway)
    {
        struct hal_network network;

        int retval = 0;

        unsigned int index = 0;//板载网卡的序号为0
        retval = hal_get_network(index, &network);
        if(0 != retval)
        {
            qDebug("CSEVR get network information failed: %d!", retval);
            return false;
        }

        struct in_addr iaddr;
        iaddr.s_addr = network.ip.ipv4.address;
        ip = inet_ntoa(iaddr);

        struct in_addr imask;
        imask.s_addr = network.ip.ipv4.netmask;
        mask = inet_ntoa(imask);

        struct in_addr igate;
        igate.s_addr = network.ip.ipv4.gw;
        gateway = inet_ntoa(igate);

        return true;
    }

#endif

};




#endif // COMMON

