# MFC_SOCKET_IMG
项目：CSOCKET通讯
开发语言：C++
开发工具：VS
项目描述：client端和server的图形界面是通过MFC下的一些控件实现的，包括Button、Pitcure Control、list Control,通信方式选择了VS中封装好了的API,
          有两种已经封装好的类，分别是CAsyncSocket及CSocket，在这个项目中我使用了CSocket
项目功能：client端将本地图片加载至界面上,可通过按钮实现对图片的选择、添加、删除、发送等基本操作，点击发送前需要先在server端开启服务,才能在
          服务器的list控件显示发送记录，server可以将记录中每一张图片显示在picture Control上
