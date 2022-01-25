import pyrebase
from datetime import datetime 
import errno

# Initialize the pyrebase
config = {
  "apiKey": "AIzaSyB8Dzaeb7D3_QhrrD4gcDuqiSfYmesyIbw",
  "authDomain": "pigate-175f8.firebaseapp.com",
  "databaseURL": "https://pigate-175f8-default-rtdb.europe-west1.firebasedatabase.app",
  "projectId": "pigate-175f8",
  "storageBucket": "pigate-175f8.appspot.com",
  "messagingSenderId": "1027851390443",
  "appId": "1:1027851390443:web:50ee2a5f98c75d2a879dae"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()


def sendEntry(PIgate_ID, Plate):

    now = datetime.now()
    timeNow = now.strftime("%H:%M:%S")
    dateNow = now.strftime("%a %b %d %Y")

    dataToSend={
        "PIgate_ID": PIgate_ID,
        "Plate": Plate,
        "Time": dateNow,
        "Date": timeNow
    }

    try:
        db.child("Entry").push(dataToSend)
        return 1
    except:
        return errno.EINVAL


def getPlates():

    plate_array = []

    try:
        plates = db.child("Car").get()
    except:
        return errno.EINVAL

    for plates in plates.each():
        plate_array.append(plates.key())

    return plate_array


def checkIsToOpen(PIgate_ID):

    reference = "Gate/" + PIgate_ID + "/isToOpen"

    try:
        gates = db.child(reference).get()
    except:
        return errno.EINVAL

    if(gates.val() == 0):

        return gates.val()
    
    else:
        
        try:
            db.child(reference).set(0)
        except:
            return errno.EINVAL

        return gates.val()
    


            
    




    
