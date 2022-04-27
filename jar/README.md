**** 软件工具客户端：

所有独立模块均以插件的形式呈现

文件目录：

     build---编译目录
     CGUMainFrame---主界面框架（通过该模块统一调用插件模块）
     include---所有共享库头文件置放位置
     lib----所有共享库
     plugins---（插件目录）
        |
        |------插件工程（例：testFormLib ，RTKPlugin）
     public----（所有共享方法模块，后续扩展预留）
     qBreakpad----软件崩溃处理模块
     source----资源文件存放目录
     
     
  插件开发说明：（例：testFormLib）
  
   #所有导出类主界面均继承baseWidget，统一基类
   
   1.class FORM_EXPORT Form : public baseWidget ---form.h
   
   2.extern"C" ---CGU_Widget.h
   
   {
   
    TEST_EXPORT void* CreateGui(const void *node);//导出接口更改
    
    TEST_EXPORT void ReleaseGui(void* gui);
    
    TEST_EXPORT std::string getPluginName();
    
    ------（后续迭代）
   }
   导出接口最基础的三个方法 （ui指针，析构函数 ，模块名称）
   
   3.pro 配置：导出库的位置
      
    CONFIG(debug, debug|release){
    DESTDIR =$$PWD/../../build/debug
    }else{
    DESTDIR =$$PWD/../../build/release}
   4.基础接口类：baseWidget(以库的形式调用继承)
     
     路径：../analyzerCGU/public/baseWidget
   
   5.插件主界面设置flag主框架关闭时自动析构，释放当前插件的资源占用

 注：**提交代码时需要把所有build生成的文件删除干净再提交，只提交各自插件的代码** 