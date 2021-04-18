# AsciiVideoCPP
A simple code using OpenCV and C++ to create a ASCII Video 

/!\ <b>Warning</b> This code can only work on Windows /!\  

You can use this code to convert a video to an ASCII video displayed in the console.  
The code is definitely not perfect and has several know issues.   
Furthermore, it could be improved to be more efficient in regards of CPU usage, but also in converting the video properly.  

It works as follow :  
1. Put in the same directory the .exe file, the video's audio and the video you want to convert.   
<b>The video must be a audioless .mp4, and the audio a .wav</b>  
To remove the video's audio, I recommend installing ffmpeg and doing `ffmpeg -i input-video -c copy -an output-video` (remove input-video and output-video by the video's file's name).  

2. Launch the .exe file : input the audioless video's file's name (without the .mp4)  
3. If it hasn't already done it, the program will download each frame of the video in a folder placed inside of the .exe's directory. The frames are in black and white.  
I recommend using this program with small videos, as it could take time. You can make test with the Bad Apple!! opening included in this repo. The video is already audioless.  
4. If it hasn't already done it, the program will then convert each frame in a .txt file. The window will be maximized, and the console font size reduced for better quality. The frames will be resized.  
5. Finally, the program will read each .txt file one by one and display it into the window, giving the illusion of a video. The folders containing the images and the text files won't be deleted, so they can be re-used again in the future if you want to rewatch the same video. Of course, you can delete them manually.  
  
Demonstration : https://www.youtube.com/watch?v=6RpQRvMbSeA&ab_channel=PtitGnou (files were already downloaded)
  
## Known issues  
 
- [x] ~~Sometimes, there are jumps between frames~~ *Resolved by using printf instead of cout, resulting in a faster display. It can still happen with some videos, but resolvable by increasing the console's font size*
- [x] ~~The program works really well with the Bad Apple!! video because it is big white and black shadows. However, the rendering is not as great with other videos. You can try lowering the font size in the "change_console_size()" function, but the console had trouble rendering as much text this fast on my laptop.~~ *Resolved by using printf instead of cout, resulting in faster display and making it possible to increase number of pixel being displayed without lag* 
- [ ] The program crashes after downloading the frames. You can simply reopen the program afterwards and skip the "Download frames" part by pressing "N".  
- [ ] The program doesn't take into consideration the general brightness of the image, which means that if the video is happening in a dark place for example, it will be hard to detect the different parts of the image.  
- [ ] Sometimes, the video loses some milliseconds, which is enough to make it seem disconnected with the audio (and finish after it).
  
## Disclaimer  

I am not a professional, and I know this program has a lot of issues.  
This project was simply a fun thing I wanted to try, and it definitely made me improve my knowledge of the C++ language.  
I first had the inspiration for this project after looking at this project in Python, using the same library : https://github.com/CalvinLoke/bad-apple  
Furthermore, some parts of my code are not original, and are taken from Stack Overflow. I tried referencing the sources as much as I could.
