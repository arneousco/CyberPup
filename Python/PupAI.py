#!/usr/bin/env python3
"""
A:Advance | L:Lexicographical
F:Functionality | R:Rhetoric and
E:Enhance | D:Dog
"""

import datetime
import random
import os
import pyttsx3
import speech_recognition as sr
import webbrowser
import wikipedia
import requests

# Setting Microsoft Speech API(SAPI5) for voice.
engine = pyttsx3.init("sapi5")
voices = engine.getProperty('voices')
engine.setProperty("voice", voices[0].id)
person = 0
a_user = "Sir"



def switch_person(p_no, user):
    global person, a_user
    person = p_no
    a_user = user
    engine.setProperty("voice", voices[p_no].id)


def speak(audio_string):
    engine.say(audio_string)
    engine.runAndWait()


def wish():
    s_morning_wishes = [f"Good Morning {a_user}, How may I help you ?", f"Well Hello {a_user}, How can I assist you ?",
                        f"Wishing you a very pleasant morning {a_user}, So how can I assist you ?",
                        f"I hope you had a good sleep {a_user}, How can I help you ?"]
    s_afternoon_wishes = [f"Good afternoon {a_user}, How can I help you ?",
                          f"Have a cup of tea {a_user}, and tell me what to do ?",
                          f"Afternoon is best to work, So how can I help you {a_user} ?",
                          f"Hope you are having a Good day {a_user}"]
    s_evening_wishes = [f"Good evening {a_user}! I hope you had a good and productive day. How may I help you ?",
                        f"Good evening {a_user}, take a sip of your coffee and tell me how can I help you.",
                        f"I hope you are having a refreshing evening {a_user}, How can I assist you ?",
                        f" Good evening {a_user}, How may I help you ?"]
    hour = datetime.datetime.now().hour
    if hour == 0 or hour < 12:
        speak(random.choice(s_morning_wishes))
    elif hour == 12 or hour < 18:
        speak(random.choice(s_afternoon_wishes))
    else:
        speak(random.choice(s_evening_wishes))



def take_input(num, wait):
    r = sr.Recognizer()
    sr.Microphone(device_index=1)
    with sr.Microphone() as source:
        print("Listening....")
        r.adjust_for_ambient_noise(source)  # Important// to dynamically calibrate threshold energy.
        r.pause_threshold = wait
        audio = r.listen(source)
    try:
        print("Recognise...")
        query = r.recognize_google(audio, language="en-in")
    except sr.RequestError:
        speak(f"I think you are Offline {a_user}")
        return "None"
    except Exception as exp:
        print(exp)
        if num == 0:
            speak('Sorry, I did not get that!')
        return "None"
    return query
from first2 import button_pressed_thread

def move(chara):
    pass
    button_pressed_thread(chara)

def weather(num):
    weather_url = \
        "http://api.openweathermap.org/data/2.5/weather?appid=54faf08469904b4809895daae7d35a29&q=Raebareli"
    response = requests.get(weather_url)
    x = response.json()
    try:
        y = x["main"]
        current_temperature = y["temp"] - 273.15
        current_pressure = y["pressure"]
        current_humidity = y["humidity"]
        z = x["weather"][0]["description"]
        if num == 1:
            message = f"Temperature is {current_temperature} degree Celsius\nHumidity is {current_humidity} percent" \
                      f"\nAtmospheric pressure is {current_pressure} hPa\nWeather status is {z}"
            print(message)
            speak(message)
        else:
            message = "The weather is: " + str(z)
            print(message)
            speak(message)
    except Exception as ex:
        print(ex)
    return None



def news():
    url = ""
    technology = \
        "http://newsapi.org/v2/top-headlines?country=in&category=technology&apiKey=b1d5b72a26d8459a9ca3303d35c3868e"
    entertainment = \
        "http://newsapi.org/v2/top-headlines?country=in&category=entertainment&apiKey=b1d5b72a26d8459a9ca3303d35c3868e"
    science = \
        "http://newsapi.org/v2/top-headlines?country=in&category=science&apiKey=b1d5b72a26d8459a9ca3303d35c3868e"
    speak(f"Science, Technology or Entertainment {a_user} ?")
    choice_w = take_input(0, 0.5).lower()
    if "entertainment" in choice_w:
        url = entertainment
    elif "science" in choice_w:
        url = science
    elif "technology" in choice_w:
        url = technology
    elif "cancel command" in choice_w and len(choice_w) == 14:
        return None
    elif choice_w == "none":
        while choice_w != "none":
            choice_w = take_input(0, 0.5).lower()
    try:
        open_page = requests.get(url).json()
        article = open_page["articles"]
        results = []
        speak(f"Here are  the top 10 Headlines {a_user}")
        for ar in article:
            results.append(ar["title"])
        for i in range(0, 10):
            print(i + 1, results[i])
            speak(f"{i + 1}, {results[i]}")
    except Exception as exception:
        print(exception)
        speak('Some error occurred.')
    return None


def troubleshoot_test():
    speak(f"OK {a_user}, Say something :")
    message = take_input(0, 1).title()
    speak(f"You said: {message}")
    return None


def oracle_intro():
    speak("Hi, I am Oracle, a intelligent quadruped robot.")
    return None


def alfred_intro():
    speak("Hi, I am Alfred, a intelligent quadruped robot.")
    return None


def sleep():
    active = False
    while not active:
        wake_up = take_input(1, 0.5).lower()
        if "alfred" in wake_up or "oracle" in wake_up:
            speak(f"Waiting for your command {a_user}")
            active = True
    return None


def day_date():
    _day_date = datetime.date.today().strftime("%d %m %y")
    day = datetime.datetime.today().weekday() + 1
    day_dict = {1: 'Monday', 2: 'Tuesday',
                3: 'Wednesday', 4: 'Thursday',
                5: 'Friday', 6: 'Saturday',
                7: 'Sunday'}
    speak(f"It's {day_dict[day]} the {_day_date}")
    return None


if __name__ == '__main__':
    switch_person(0, "Sir")
    wish()
    leave = False
    while not leave:
        Query = take_input(0, 1).lower()
        if "alfred" in Query[0:7]:
            Query = Query.replace("alfred", '')

        if "oracle" in Query[0:7]:
            Query = Query.replace("oracle", '')
        print(f"You said: {Query}")

        if "search about" in Query:
            speak("Searching")
            print("Searching...")
            Query = Query.replace("search about", "")
            try:
                result = wikipedia.summary(Query, sentences=2)
                print(result)
                speak(result)

            except wikipedia.DisambiguationError as dis:
                random_topic = random.choice(dis.options)
                print("Showing random result:")
                print(len(dis.options))
                result = wikipedia.summary(random_topic, sentences=2)
                speak(result)
            except Exception as e:
                print(e)
                speak("Hmm, An unexpected error occurred")


        elif "time" in Query and len(Query) == 4:
            str_time = datetime.datetime.now().strftime("%H:%M:%S")
            speak(f"{a_user} the time is: {str_time}")

        elif "tell me the day" in Query or "tell me the date" in Query:
            day_date()

        elif "wish me" in Query and len(Query) <= 15:
            speak(f"As you wish {a_user}")
            wish()
        elif "move" in Query and len(Query) <= 15:
            if 'up' in Query:
                move('A')
            elif 'down' in Query:
                move('B')
        elif 'pitch' in Query:
            if 'forward' in Query:
                move('C')
            elif 'backward' in Query:
                move('D')
        elif 'slide' in Query:
            if 'forward' in Query:
                move('P')
            elif 'backward' in Query:
                move('Q')

        elif "tell me some news" in Query:
            news()

        elif "perform a hearing test" in Query or "perform hearing test" in Query:
            troubleshoot_test()


        elif "switch to oracle" in Query:
            switch_person(1, "Sir")
            speak(f"Hi {a_user}!, Oracle Here. How can I help you ?")

        elif "switch to alfred" in Query:
            switch_person(0, "Sir")
            speak("Alfred at your service Sir.")

        elif "sleep" in Query:
            speak("Let me know when you need me.")
            sleep()

        elif "introduce yourself" in Query:
            if person == 0:
                alfred_intro()
            else:
                oracle_intro()


        elif "tell me weather report" in Query or "tell me the weather" in Query:
            speak(f"Full report or just weather report {a_user} ?")
            choice = take_input(0, 0.5).lower()
            if choice == "full report" or choice == "full weather report" or choice == "full":
                speak(f"OK {a_user}")
                weather(1)
            elif choice == "just weather" or choice == "just weather report":
                speak(f"Very well {a_user}")
                weather(0)
            else:
                speak(f"I did not get that {a_user}")

