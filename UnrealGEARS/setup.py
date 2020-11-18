import requests
import zipfile
import os
import os.path

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

projectRoot = os.path.dirname(os.path.realpath(__file__))
lammpsDumpDir = os.path.join(projectRoot, 'LammpsEditor', 'Content', 'LammpsResource', 'LammpsDump')
if not os.path.exists(lammpsDumpDir):
  os.makedirs(lammpsDumpDir)

lammpsDumpFiles = os.listdir(lammpsDumpDir)
lammpsDumpFilename = 'LammpsDumpFiles.zip'
googleDriveFileId = '1xZjoNFEQ-1oVfaBIQ-zNmSEHEIEqOlk6' # hard coded file id
lammpsDumpZipFullpath = os.path.join(lammpsDumpDir, lammpsDumpFilename)

if len(lammpsDumpFiles) == 0:
    print('Downloading LAMMPS dump files for Animation Demo')
    download_file_from_google_drive(googleDriveFileId, lammpsDumpZipFullpath)
    print('Download finished. Extracting files')
    zip = zipfile.ZipFile(lammpsDumpZipFullpath, 'r')
    zip.extractall(lammpsDumpDir)
    zip.close()
    print('Setup complete')
else:
    print('Demo 2 Geometry Data already exists. If prefab errors occur, remove folder and meta files, then retry.') 