#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <chrono>
#include <thread>
#include <winuser.h>
#include <cwchar>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;
using namespace System;

void change_console_size(int size) {

    CONSOLE_FONT_INFOEX cfi; //https://stackoverflow.com/questions/35382432/how-to-change-the-console-font-size
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = size;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

}

void maximize_window() { //https://stackoverflow.com/questions/5300170/maximize-console-windows-in-windows-7/5300345#5300345
    HWND hWnd;
    SetConsoleTitle(L"ASCII Video");
    hWnd = FindWindow(NULL, L"ASCII Video");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
    SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
    
    SetConsoleScreenBufferSize(hOut, NewSBSize);

    DisplayArea.Right = NewSBSize.X - 1;
    DisplayArea.Bottom = NewSBSize.Y - 1;

    SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);

    ShowWindow(hWnd, SW_MAXIMIZE);
}

void clear_screen(char fill = ' ') { //Function to clear console output https://stackoverflow.com/questions/5866529/how-do-we-clear-the-console-in-assembly/5866648#5866648
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

void displayFrame(int numberOfFrames, string textPath, const int frameRate, int size) {
    string framePath;
    string veryFinalDisplay;

    string musicName;

    change_console_size(size);
    maximize_window();

    std::cout << "What is the music name ? Press d for default, n for no music. Must be a wav file." << endl;
    cin >> musicName;

    if (musicName == "d") {
        musicName = "bad-apple-music";
    }
    musicName += ".wav";

    fs::path musicPath = fs::current_path().parent_path();
    musicPath += "\\";
    musicPath += musicName;
    string musicPathStr = musicPath.string();

    clear_screen();
    
    if (musicName != "n.wav") {
        PlaySoundA(musicPathStr.c_str(), NULL, SND_ASYNC);
    }
    

    int FPS = frameRate; //Huge thanks to Ted Lyngmo https://stackoverflow.com/questions/65112289/sleep1-sleep-more-than-1-millisecond-windows-10-20h2-problem/65112657#65112657
    auto time_between_frames = std::chrono::microseconds(std::chrono::seconds(1)) / FPS;

    auto target_tp = std::chrono::steady_clock::now();

    for (int t = 1; t <= numberOfFrames; t++) {


        framePath = textPath + to_string(t) + ".txt";

        ifstream stream(framePath);
        stringstream strStream;
        strStream << stream.rdbuf();
        veryFinalDisplay = strStream.str();
        stream.close();

        printf("\r");
        printf("%s", veryFinalDisplay.c_str());
        printf("\n");

        target_tp += time_between_frames;          
        std::this_thread::sleep_until(target_tp);

    }
}

void loadFrame(Mat image, int frameNumber, int size) {

    if (frameNumber == 1) {
        fs::path pathToGo = fs::current_path();
        pathToGo += "\\TextDir\\";

        if (!fs::exists("TextDir")) {
            fs::create_directory("TextDir");
            fs::current_path(pathToGo);
        }
        else {
            fs::remove_all(pathToGo);
            fs::create_directory("TextDir");
            fs::current_path(pathToGo);
        }
    }

    string finalDisplay = "";

    Mat imageResized;

    maximize_window();

    change_console_size(size);

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int w_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int w_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    double height_ratio, width_ratio;

    /*cout << "Console size : " << w_width << "/" << w_height << endl;
    cout << "Image size : " << image.size().width << "/" << image.size().height << endl;*/

    height_ratio = (double)w_height / image.size().height;
    
    width_ratio = (double)w_width / image.size().width;

    /*cout << width_ratio << endl;
    cout << height_ratio << endl;*/

    resize(image, imageResized, Size(), width_ratio, height_ratio);

    for (int y = 0; y < imageResized.rows; y++)
    {
        for (int x = 0; x < imageResized.cols; x++)
        {
            Vec3b color = imageResized.at<Vec3b>(Point(x, y));

            int colorInt = color[0];

            if (colorInt >= 200) {
                colorInt += 35;
                if (colorInt >= 255) {
                    colorInt = 255;
                }
            }
            else if (colorInt <= 120) {
                colorInt -= 40;

                if (colorInt <= 0) {
                    colorInt = 0;
                }
            }

            //ASCII_CHARS = ["@", "#", "S", "%", "?", "*", "+", ";", ":", ",", "."]

            if (colorInt <= 18) {
                finalDisplay += "@";
            }
            else if (colorInt <= 30) {
                finalDisplay += "#";
            }
            else if (colorInt <= 50) {
                finalDisplay += "S";
            }
            else if (colorInt <= 74) {
                finalDisplay += "%";
            }
            else if (colorInt <= 100) {
                finalDisplay += "?";
            }
            else if (colorInt <= 125) {
                finalDisplay += "*";
            }
            else if (colorInt <= 145) {
                finalDisplay += "+";
            }
            else if (colorInt <= 170) {
                finalDisplay += ";";
            }
            else if (colorInt <= 190) {
                finalDisplay += ":";
            }
            else if (colorInt <= 220) {
                finalDisplay += ",";
            }
            else if (colorInt < 255) {
                finalDisplay += ".";
            }
            else if (colorInt == 255) {
                finalDisplay += " ";
            }
        }

        finalDisplay += "\n";
    }

    finalDisplay.resize(finalDisplay.size() - 1);

    string fileName = "txt" + to_string(frameNumber) + ".txt";

    ofstream outfile(fileName);
    outfile << finalDisplay;
    outfile.close();

    finalDisplay.clear();
}

void addToDir(Mat image, int frameNumber, string parentDirName) {

    if (frameNumber == 1) {

        fs::path pathToGo = fs::current_path();
        pathToGo += "\\";
        pathToGo += parentDirName;
        pathToGo += "\\";

        if (!fs::exists(parentDirName)) {
            fs::create_directory(parentDirName);
            fs::current_path(pathToGo);
        }
        else {
            fs::remove_all(pathToGo);
            fs::create_directory(parentDirName);
            fs::current_path(pathToGo);
        }

        pathToGo += "framesDir\\";

        if (!fs::exists("framesDir")) {
            fs::create_directory("framesDir");
            fs::current_path(pathToGo);
        }
        else {
            fs::remove_all(pathToGo);
            fs::create_directory("framesDir");
            fs::current_path(pathToGo);
        }
    }

    Mat imageGray;

    cvtColor(image, imageGray, COLOR_RGB2GRAY);

    string frameName = "frame" + to_string(frameNumber) + ".png";

    imwrite(frameName, imageGray);

}

void handleVideo(VideoCapture cap, string videoName){

    Mat frame;
    int frameNumber = 1;
    string progressBar;
    char downloaded;
    int size = 6;

    videoName.resize(videoName.size() - 4);

    clear_screen();

    if (cap.isOpened()) {
        std::cout << "Video Working" << endl;
    }

    int frameRate = cap.get(CAP_PROP_FPS);
    int totalFrames = cap.get(CAP_PROP_FRAME_COUNT);
    string frameRateInput;

    std::cout << "The video is at " << frameRate << " FPS." << endl;
    std::cout << "There are " << totalFrames << " frames in this video." << endl;

    if (fs::exists(videoName)) {
        std::cout << "You seem to already have the image files. Do you want to re-download them ? (Y/N)" << endl;
        cin >> downloaded;
    }
    else {
        downloaded = 'Y';
    }

    if (downloaded == 'Y') {

        cout << "The image files will now be downloaded. This could take some time." << endl;

        while (true) {
            cap >> frame;

            if (frame.empty())
                break;

            addToDir(frame, frameNumber, videoName);

            if (frameNumber == 1) {
                clear_screen();
            }

            if (frameNumber % (totalFrames / 10) == 0) {
                progressBar += "#";
                std::cout << "\rProgression : " << progressBar;
            }

            std::cout << "\rProgression : " << progressBar << " Frame " << frameNumber << " of " << totalFrames;

            frameNumber++;

            char c = (char)waitKey(25);
        }

        std::cout << "\nDone" << endl;

        cap.release();
        
    }

    downloaded = 'E';

    frameNumber = 1;
    progressBar = "";
    Mat currentFrame;
    string imagePathStr;

    fs::path videoPath = fs::current_path();
    videoPath += "\\";
    videoPath += videoName;
    videoPath += "\\";
    fs::current_path(videoPath);

    if (fs::exists("TextDir")) {
        std::cout << "You seem to already have the text files. Do you want to re-download them ? (Y/N)" << endl;
        cin >> downloaded;
    }
    else {
        downloaded = 'Y';
    }

    bool wroteCurrentFile = FALSE;

    if (downloaded == 'Y') {

        char changeSize;

        cout << "The default font size is 6. The lower it is, the better the video's quality will be. We recommend not going lower than 6, as this could cause lag on the video. If 6 is already causing lag, we recommend going up to 8 or 10. Do you want to change font size ? (Y/N)" << endl;
        cin >> changeSize;

        if (changeSize == 'Y') {
            cout << "Please insert the size you wish" << endl;
            cin >> size;
        }

        std::cout << "Downloading text files..." << endl;
        for (int n = 1; n <= totalFrames; n++) {

            fs::path imagePath = fs::current_path();

            imagePathStr = imagePath.string();

            if (n != 1) {
                imagePathStr.resize(imagePathStr.size() - 8);
            }

            imagePathStr += "\\framesDir\\frame";
            imagePathStr += to_string(n);
            imagePathStr += ".png";


            std::replace(imagePathStr.begin(), imagePathStr.end(), '\\', '/');

            currentFrame = imread(imagePathStr);

            if (currentFrame.empty()) {
                std::cout << imagePathStr << " - Could not load image" << endl;
            }
            else {
                loadFrame(currentFrame, frameNumber, size);
            }

            if (frameNumber % (totalFrames / 10) == 0) {
                progressBar += "#";
            }

            std::cout << "\rProgression : " << progressBar << " Frame " << frameNumber << " of " << totalFrames;
            frameNumber++;

            if (frameNumber == totalFrames) {
                break;
            }
        }

        fs::current_path(fs::current_path().parent_path());
        wroteCurrentFile = TRUE;
    }

    fs::path sizePath = fs::current_path();
    sizePath += "\\size.txt";
    int sizeInFile;

    if (fs::exists(sizePath)) {
        ifstream streamSize(sizePath);
        stringstream sizeStream;
        sizeStream << streamSize.rdbuf();
        sizeInFile = stoi(sizeStream.str());
        streamSize.close();

        if (wroteCurrentFile) {
            fs::remove_all(sizePath);
            ofstream outfileText("size.txt");
            outfileText << size;
            outfileText.close();
        }
        else {
            size = sizeInFile;
        }
    }
    else {
        ofstream outfileText("size.txt");
        outfileText << size;
        outfileText.close();
    }

    fs::path textPath = fs::current_path();
    string textPathStr = textPath.string();
    textPathStr += "\\TextDir\\txt";
    std::replace(textPathStr.begin(), textPathStr.end(), '\\', '/');

    std::cout << "\nAll files downloaded. Launching the video... \n";

    displayFrame(totalFrames, textPathStr, frameRate, size);
}

int main()
{
    system("color F0"); //Sets the console color to white, change this values to change the console color as you wish
    //First digit/letter is background, second is text color.
    /*0 = Black        8 = Gray
    1 = Blue        9 = Light Blue
    2 = Green        A = Light Green
    3 = Blue-Gray        B = Cyan
    4 = Redd      C = Light Red
    5 = Purple     D = Light Purple
    6 = Yellow        E = Light Yellow
    7 = White       F = Clear white */

    string videoName;

    clear_screen();

    std::cout << "What is the video name ? Press d for default. Must be a mp4 audioless file." << endl;
    cin >> videoName;

    if (videoName == "d") {
        videoName = "bad-apple";
    }

    videoName += ".mp4";

    VideoCapture cap(videoName);

    handleVideo(cap, videoName);

    return 0;
}