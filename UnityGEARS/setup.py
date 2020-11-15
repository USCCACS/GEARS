#!/usr/bin/env python

import argparse
import zipfile
import os
import os.path
import subprocess
import sys
import requests

# Credit where credit is due: https://stackoverflow.com/a/39225039
def download_file_from_google_drive(id, destination):
    def get_confirm_token(response):
        for key, value in response.cookies.items():
            if key.startswith('download_warning'):
                return value

        return None

    def save_response_content(response, destination):
        CHUNK_SIZE = 32768

        with open(destination, "wb") as f:
            for chunk in response.iter_content(CHUNK_SIZE):
                if chunk: # filter out keep-alive new chunks
                    f.write(chunk)

    URL = "https://docs.google.com/uc?export=download"

    session = requests.Session()

    response = session.get(URL, params = { 'id' : id }, stream = True)
    token = get_confirm_token(response)

    if token:
        params = { 'id' : id, 'confirm' : token }
        response = session.get(URL, params = params, stream = True)

    save_response_content(response, destination)    

#Acquire the current working directory
currentDir = os.path.dirname(os.path.realpath(__file__))

# Parse Arguments
parser = argparse.ArgumentParser()
parser.add_argument("--noVCM", help="Skips setup for the Virtual Confocal Microscopy (Demo 2)", action="store_true")

args = parser.parse_args();

# Unzip LeapMotion and Geometry Data Files
projectDir = './Editor/'
fullProjectPath = currentDir + projectDir
assetsPath = projectDir + 'Assets/'

geometryDir = 'GeometryData/'
geometryMeta = 'GeometryData.meta'
geometryZip = 'GeometryData.zip'

leapZipPath =  assetsPath + 'Leap_Motion_CoreAssets_4.2.1.zip'
leapDir = assetsPath + 'LeapMotion/'
leapMeta = assetsPath + 'LeapMotion.meta'
demo2Dir = assetsPath + 'Demo2-VirtualConfocalMicroscopy/'
demo2GeometryZipPath = demo2Dir + geometryZip

if not os.path.isdir(leapDir) and not os.path.isfile(leapMeta):
	print('Unzipping Leap Motion Package')
	leapZip = zipfile.ZipFile(leapZipPath, 'r')
	leapZip.extractall(assetsPath)
	leapZip.close()

if not args.noVCM:
    if not os.path.isdir(demo2Dir + geometryDir) and not os.path.isfile(demo2Dir + geometryMeta):
        print('Unzipping Virtual Confocal Microscopy geometry data')
        geometryDataFileId = '1E0kapjSMS-4Wl6bqg2ERAD6R8xWKcxtx' # hard coded file id
        download_file_from_google_drive(geometryDataFileId, demo2GeometryZipPath)
        demo2Zip = zipfile.ZipFile(demo2GeometryZipPath, 'r')
        demo2Zip.extractall(demo2Dir)
        demo2Zip.close()
    else:
        print('Demo 2 Geometry Data already exists. If prefab errors occur, remove folder and meta files, then retry.') 

print('Setup Complete')
