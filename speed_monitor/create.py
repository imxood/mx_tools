# coding:utf-8

import sys
import psutil

from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *

import datetime

# 查看cpu的信息
print u"CPU 个数 %s" % psutil.cpu_count()
print u"物理CPU个数 %s" % psutil.cpu_count(logical=False)
print u"CPU uptimes"
print psutil.cpu_times()
print ""

# 查看内存信息

memory = psutil.virtual_memory()
print u"系统总内存 %s M" % (memory.total / 1024 / 1024)
print u"系统可用内存 %s M" % (memory.available / 1024 / 1024)
mem_rate = int(memory.available) / float(memory.total)
print u"系统内存使用率 %s %%" % int(mem_rate * 100)

# 系统启动时间
print u"系统启动时间 %s" % datetime.datetime.fromtimestamp(psutil.boot_time()).strftime("%Y-%m-%d %H:%M:%S")

# 系统用户
users_count = len(psutil.users())
users_list = ",".join([u.name for u in psutil.users()])
print u"当前有%s个用户，分别是%s" % (users_count, users_list)

# 网卡，可以得到网卡属性，连接数，当前流量等信息
net = psutil.net_io_counters()
bytes_sent = '{0:.2f} kb'.format(net.bytes_recv / 1024)
bytes_rcvd = '{0:.2f} kb'.format(net.bytes_sent / 1024)
print u"网卡接收流量 %s 网卡发送流量 %s" % (bytes_rcvd, bytes_sent)

app = QApplication(sys.argv)

desktop = QApplication.desktop()

quitAction = QAction(u"退出", desktop)

menu = QMenu(desktop)
menu.addAction(quitAction)

imageText = "HELLO"

font = QFont()
# 设置显示字体的大小
font.setPixelSize(13)

fontMetrics = QFontMetrics(font)

charWidth = fontMetrics.width(imageText)

charWidth = fontMetrics.boundingRect(imageText).width()

# 指定图片大小为字体的大小
size = QSize(charWidth+8, 15)

# 以ARGB32格式构造一个QImage
image = QImage(size, QImage.Format_ARGB32)
# 填充图片背景,120/250为透明度
image.fill(qRgba(255, 255, 255, 0))
# 为这个QImage构造一个QPainter
painter = QPainter(image)

# 设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
# 改变组合模式和上面的填充方式可以画出透明的图片。
painter.setCompositionMode(QPainter.CompositionMode_DestinationOver)

# 改变画笔和字体
pen = painter.pen()
pen.setColor( QColor(0, 141, 239) )

painter.setPen(pen)
painter.setFont(font)

# 将Hello写在Image的中心
painter.drawText(image.rect(), Qt.AlignCenter, imageText);

if QSystemTrayIcon.isSystemTrayAvailable():
    trayIcon = QSystemTrayIcon()
    trayIcon.setIcon(QIcon(QPixmap(image)))
    trayIcon.setToolTip(u"软件中心")
    trayIcon.showMessage(u"托盘", u"托盘管理", QSystemTrayIcon.Information, 10000)
    trayIcon.setContextMenu(menu)
    trayIcon.show()

sys.exit(app.exec_())
