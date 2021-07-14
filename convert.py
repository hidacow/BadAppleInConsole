import matplotlib.pyplot as plt
import cv2
from pathlib import Path

ascii_char = list("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ")
char_len = len(ascii_char)
show_heigth = 49    #帧高度
show_width = 112    #帧宽度
VIDEO_PATH = "data/1.flv"

def convert(pic):
    pic_heigth, pic_width, _ = pic.shape
    gray = 0.2126 * pic[:, :, 2] + 0.7152 * pic[:, :, 1] + 0.0722 * pic[:, :, 0]
    res = ""
    for i in range(show_heigth):
        y = int(i * pic_heigth / show_heigth)
        text = ""
        for j in range(show_width):
            x = int(j * pic_width / show_width)
            text += ascii_char[int(gray[y][x] / 256 * char_len)]
        res+=text+'\n'
    return res


def write(VIDEO_PATH):
    print("转换视频中...")
    vc = cv2.VideoCapture(VIDEO_PATH)
    count = 0
    f, frame = vc.read()
    video_data = ""
    while f:
        count += 1
        print("\r转换进度: %d" % count, end='')
        text = convert(frame)
        video_data+=text
        f, frame = vc.read()

    with open("data.txt", "w") as f:
        f.write(video_data)

    return video_data


if __name__ == '__main__':
    if not Path(VIDEO_PATH).exists:
        print("视频不存在: ", VIDEO_PATH)
    video_data = write(VIDEO_PATH)
    print("\n转换完成")
