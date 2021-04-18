from pytube import YouTube
from youtubesearchpython import VideosSearch
import ffmpeg
import os

videoName = input("What's the video name to be searched on Youtube ? ")
clean_videoName = videoName.replace(" ", "-")

yt = VideosSearch(videoName, limit=5)

result_array = yt.result().get("result")

i = 1

for value in result_array:
    print(i, ". ", value["title"])

    i += 1

video_chosen_index = input("Please choose a video from the list by inputting its index \n")
video_chosen = result_array[int(video_chosen_index) - 1]

print(video_chosen["title"])
print(video_chosen["link"], "\n")

yt = YouTube(video_chosen["link"])
yt_filtered = yt.streams.filter(progressive=False, file_extension='mp4', only_video=True)

i = 1

for streams in yt_filtered:
    print(i, ". ", streams)
    i += 1

video_stream_chosen_id = input("\n\nPlease choose a video stream from the list \n")
video_stream_chosen = yt_filtered[int(video_stream_chosen_id) - 1]

audio = YouTube(video_chosen["link"])
audio_filtered = audio.streams.filter(progressive=False, only_audio=True, file_extension="webm")

i = 1

for streams in audio_filtered:
    print(i, ". ", streams)
    i += 1

audio_stream_chosen_id = input("\nPlease choose an audio stream from the list \n")
audio_stream_chosen = audio_filtered[int(audio_stream_chosen_id) - 1]

print("\nFiles are being downloaded...")

clean_videoName = input("\nWhat do you want the filenames to be called ? We recommend giving it a short name, as it could result in a faster download of the files\n")

video_stream_chosen.download(filename=clean_videoName)
audio_stream_chosen.download(filename=clean_videoName)

audio_name = clean_videoName + ".webm"
audio_name_output = clean_videoName + ".wav"

convert = ffmpeg.input(audio_name)
convert = ffmpeg.output(convert, audio_name_output)
ffmpeg.run(convert)

os.remove(audio_name)

print("Everything downloaded.")
print("For the next steps, use ", clean_videoName, " as the video and music name. \n")
