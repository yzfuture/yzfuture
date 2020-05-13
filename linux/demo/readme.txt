1.修改Config.ini文件在FactoryFlag后面添加自己的appkey
FactoryFlag=你的appkey

2.在终端中输入以下指令
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./

或者将libreadCardInfo.so和libwlt.so两个文件拷贝到/usr/lib目录下即可

3.使用make命令编译demo

4.连接读卡器，放上身份证，保证网络畅通

5.运行./test即可
