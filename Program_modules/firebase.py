import pyrebase
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
auth = firebase.auth()

#login
def initFirebase():

    email = "PIgate.project@gmail.com"
    password = "PIgate"

    try:
        auth.sign_in_with_email_and_password(email,password)
        print("Login Success")
        return 1

    except:
        print("Bad Login")
        return errno.EINVAL



def sendEntry(PIgate_ID, Plate, Time, Date):

    dataToSend={
        "PIgate_ID": PIgate_ID,
        "Plate": Plate,
        "Time": Time,
        "Date": Date
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

    print(plate_array)

    return plate_array

