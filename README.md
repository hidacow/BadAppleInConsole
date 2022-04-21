# Bad Apple in Console

## **Introduction**

This program plays Bad Apple (ASCII Art) in Windows console synchronised with music.

This repository also contains a tool ([convert.py](convert.py)) to convert any video into ASCII Art stored in text files.

This repo is part of the group project for the SHU Summer "Programming Training" Course

## **Quick Start**

### **Open with Visual Studio(Recommended)**

This repository contains the VS2019 project file.

The easiest way to open this project is to install Visual Studio 2019 and open `BadApple.sln`

### **Open with CodeBlocks**

This repository contains the CodeBlocks project file.

You may open this project by installing CodeBlocks and open `BadApple.cbp`

### **Notice**

- We use Windows API to achieve this program, and therefore this program runs on **Windows Only**
- We tested building this program with MSVC(19.29.30038.1) and g++(8.1.0)
- The code(comments, output) contains Chinese
- Source files are encoded with GB2312
- You may not copy any part of this project and use it in your **SHU course project**

## **Convert Tool**

You may use `convert.py` to convert any video into ASCII Art stored in text files.

### **Install Modules**

```bash
python -m pip install -r requirements.txt
```

### **Edit the code**

You may change video file path, frame height, frame width to meet your needs.

You may also edit the code in `BadApple.cpp` to adapt to new videos.

### **Run the Program**

```bash
python convert.py
```

After converting, you may see the text file containing the video in ASCII art form.

## **Other**

This project is inspired by and uses part of

 [CallMeToProgram/Bad-Apple](https://github.com/CallMeToProgram/Bad-Apple)

 [laike9m/Bad-Apple-Console](https://github.com/laike9m/Bad-Apple-Console)

## **Disclaimer**

This project is for study and recreational use only.
