# Perfmonbar

[![Build status](https://img.shields.io/appveyor/ci/XhmikosR/perfmonbar/master.svg)](https://ci.appveyor.com/project/XhmikosR/perfmonbar/branch/master)
[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/1105.svg)](https://scan.coverity.com/projects/1105)

Website: <https://xhmikosr.github.io/perfmonbar/>

<img src="https://xhmikosr.github.io/perfmonbar/img/screenshot@2x.jpg" alt="Screenshot" width="520">

## Notes

* You will have to edit the default [`config.xml`](https://github.com/XhmikosR/perfmonbar/blob/master/src/config.xml) to match your system.
* The default `config.xml` will **not** work for **non-english** systems. You have to adjust the config file, see below.
* For the known issues see [issues](https://github.com/XhmikosR/perfmonbar/issues).

## Configuration

The `config.xml` file must be adapted in the following cases:

* You have a non-english windows system running. The search strings in the `config.xml` file must be adapted to the language of the operating system, otherwise the corresponding strings will not be found by Perfmonbar and nothing will be displayed in the taskbar.
* You want to display other information in the taskbar. To do this, you have to find the appropriate and available strings and change them in the `config.xml` file.
* You want to change the appearance of the information in the taskbar. To do this, you need to change the corresponding properties in the `config.xml` file.

### Where is the `config.xml` file?

Usually the `config.xml` file can be found under:

```text
C:\Users\{User}\AppData\Roaming\PerfmonBar\config.xml
```

It may differ on your system. To quickly get to the directory, you can simply type `%appdata%` in the Windows search bar and go to the subfolder `PerfmonBar`.

### How to find the desired settings?

Open a console and run the command `typeperf -q > counters.txt`

Then you can search the file `counters.txt` for the desired performance counters that are available on your computer. This can be a bit tedious to find the right one, because there are many.
And the counters are unfortunately called differently in every single language. I don't know what Microsoft was thinking?
