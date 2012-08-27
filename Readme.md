Displays performance counters on the Windows Taskbar. This is a replacement for CureInfo.
It has support for xml configuration and is compatible with Vista/7 (Taskbar Translucency, etc...)

Original PerfmonBar Page: http://sourceforge.net/projects/perfmonbar/

# Requirements
 * Windows XP SP3 or newer

# Screenshots
http://perfmonbar.googlecode.com/files/PerfmonBar_screenshot1.png

http://perfmonbar.googlecode.com/files/PerfmonBar_screenshot2.png

http://perfmonbar.googlecode.com/files/PerfmonBar_screenshot3.png

# Notes
 * You will have to edit the default config.xml to match your system.

# TODO
 * Handle the case where the xml files do not have an Edit option and ShellExecute fails to open the configuration file (maybe just use notepad if available). Currently you can use the start menu shortcut which is using notepad to open config.xml.
 * There's a bug somewhere which causes PerfmonBar not to work on non English systems (I can reproduce it on Greek Windows XP/7). If anyone knows how to fix it, please provide a patch in the issue tracker.
