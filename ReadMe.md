当矿机太多时管理起来很麻烦，因此想完成一个批量矿机管理软件。

目前已完成一个客户端的一个小功能以，解决多用户同时使用的PC的挖矿问题：
    
	我们不希望当我们挖矿时影响别人使用计算机。
	所以只有当没有用户使用计算机时，并且CPU活动低则可启动指定后台挖矿任务。

Github地址：https://github.com/lastagile/RunCPUMiner

说明：
=================
Run.exe 为主程序，X秒钟检查一次有没有用户活动。
当在若干分钟内没有用户的鼠标或键盘的输入并且CPU 0使用率低。则会启动ToRun.bat程序。
一旦检测到有用户活动则关闭ToRun.bat以及ToRun.bat启动的子程序（包括挖矿程序）:

	无用户鼠标键盘输入后Y分钟 并且
	  CPU利用率小于Z            ->     开始执行ToRun.bat
				^                                  |
				|                                  V
		停止执行ToRun.bat           <-       有用户输入

已实现Windows下的功能：
---------------------------
	1、后台挖矿选项
	2、检测用户登录
	3、检测CPU使用率
	4、开机启动选项
		
使用方法：
===================
	根据自己的机器，选择x64或x86目录下的run.exe。
	将run.exe和config目录下的所有文件放到挖矿程序的目录中。

	startWithSystem.bat可以将run.exe设为开机启动项。

	ToRun.bat为挖矿的命令，可以在前面添加：
		echo %date% >>runTime.txt
		echo %time% >>runTime.txt
	以便查看ToRun.bat什么时候运行过，后续为自定义的挖矿命令。
	在自定义的挖矿命令中可以将挖矿程序的输出导入到文件中去，如：
		jhProtominer -o 112.124.13.238:28988 -u PYAYBuu8NQCq4P15Vg2gPjBUwebz1NCY41 -p x -t 2 1>out.txt 
	其中1>out.txt 可以将jhProtominer 的输出导出到out.txt，不过，不能立马显示，只有输出足够多时才能看到。

	Config.txt为run.exe的配置程序：
		第一行为是否显示界面（0为显示界面，1为后台运行,调试时可设为0，一般设为1）
		第二行为检测用户活动周期（X）(单位毫秒,默认3秒钟,一般不用修改)
		第三行为没用用户活动多长时间（Y）后启动ToRun.bat(单位毫秒,默认6秒，可以改为600000（10分钟）)
		第四行为当CPU0的占用率百分比（Z）小于多少才运行ToRun.bat（设为101时则，不考虑CPU占用率直接执行ToRun.bat）

后续工作：
=================================
	一、添加配置功能，如ToRun.bat启动时间选项，比如晚上10点到早上7点，再比如白天自动运行的时候挖矿用6线程，晚上用8线程。
	二、增加矿机的远程管理程序。
		1.收集矿机的CPU使用率
		2.收集矿机的挖矿效率
		3.批量远程更新矿机的配置，如挖矿程序、挖矿命令和Run.exe等
	三、增加Linux版本。

捐赠：
========================
为了支持后续的版本的发布，请支持开发工作，下面是捐赠地址。
PTS：
	
[PYAYBuu8NQCq4P15Vg2gPjBUwebz1NCY41](http://btsblock.com/address/PYAYBuu8NQCq4P15Vg2gPjBUwebz1NCY41)

QT 版本百度网盘下载：
[RunCpuMiner](http://pan.baidu.com/s/1jGhuX0M)

QQ群(提供最新QT图形化版本)： 309947822
	
