https://fast-dds.docs.eprosima.com/en/v2.7.1/installation/sources/sources_windows.html
DDS 环境配置 （https://github.com/orgs/eProsima/）（https://github.com/seznam/fastrpc.git）
依次执行如下命令：
sudo apt install libasio-dev libtinyxml2-dev（Asio 是一个用于网络和低级 I/O 编程的跨平台 C++ 库，它提供了一致的异步模型。TinyXML2 是一个简单、小巧且高效的 C++ XML 解析器）
sudo apt install libssl-dev（OpenSSL 是用于 TLS 和 SSL 协议的强大工具包和通用加密库。使用相应 Linux 发行版的包管理器安装OpenSSL）
pip3 install -U colcon-common-extensions vcstool（colcon是一个基于CMake的命令行工具，旨在构建软件包集）

源码包安装：

    git clone https://github.com/eProsima/foonathan_memory_vendor.git
  
    git clone https://github.com/eProsima/Fast-CDR.git
     
    git clone https://github.com/eProsima/Fast-DDS.git
    
    
    git clone https://github.com/eProsima/IDL-Parser.git
  
    git clone https://github.com/eProsima/Fast-RTPS
    
    git clone https://github.com/foonathan/memory.git
    cmake .. 
    make
    sudo make install
配置 Fast-DDS-Gen
 git clone https://github.com/eProsima/Fast-DDS-Gen.git
 
    cd Fast-DDS-Gen
    gradle assemble
    ./gradlew assemble
    
    配置系统环境变量
    vi ～/.bashrc
    export PATH=/home/tc/tc/DDS/Fast-DDS-Gen/scripts:$PATH
    
    fastddsgen HelloWorld.idl （编译成C++文件）
    
boost安装：（1.77）
    
  sudo ./bootstrap.sh

  sudo ./b2

  sudo ./b2 install 
安装eProsima RPC over DDS 
git clone https://github.com/eProsima/RPC.git

当前克隆的版本不能使用
需要回退版本到 之前版本 （git reset --hard ID）//Update to Fast DDS v2.0.0 (#11)
mkdir build
cd build
cmake -DRPCPROTO=rpcdds ..
make
make install

crash 环境搭建：

 （1）下载Breakpad源码
下载地址：https://github.com/google/breakpad
（2）下载LSS源码
下载地址：https://github.com/ithaibo/linux-syscall-support

（3）下载qBreakpad源码

下载地址：https://github.com/buzzySmile/qBreakpad

将linux_syscall_support.h 放入 ../breakpad/src/third_party/lss

编译breakpad ： ./configure make && make install
qt 编译 qBreakpad：
然后，将libqBreakpad.a拷贝至，qBreakpad\lib\linux目录下。

再将调用库所需的头文件QBreakpadHandler.h、QBreakpadHttpUploader.h、call_once.h、singleton.h共4个文件拷贝至qBreakpad\include下。

//生成crash 
 1. dump_syms test_crash >test_crash.so.sym
 2. minidump_stackwalk 5b48290b-0156-45d9-4b829f88-42f32168.dmp  test_crash.so.sym > crash.log
 
 //删除中文乱码文件
 ls -i （查询文件节点编号）
 find -inum  221469  -delete //删除文件
//.crt 转 .pem
openssl x509 -in mycert.crt -out mycert.pem -outform PEM
//生成密钥对
ssh-keygen -t rsa -b 4096 这里笔者加了-b 参数，指定了长度，也可以不加-b参数，直接使用ssh-keygen -t rsa，ssh-keygen命令的参数后文再介绍。

/*
 *生成1024位rsa私钥，保存为pem格式：
 *  openssl genpkey -out prikey.pem -algorithm rsa


生成对应的公钥：
openssl pkey -in prikey.pem -pubout -out pubkey.pem

*/

编译.c 
${CC} iwall_api_test.c -liwall_tbox_se -L include/

echo ${CC}

