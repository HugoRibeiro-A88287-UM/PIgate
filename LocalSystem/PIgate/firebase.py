from sre_constants import SUCCESS
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


"""
 * @brief Sends an Entry to the database
 * 
 * @param PIgate_ID 
 * @param Plate 
 * @return -EINVAL: if and error occurs ;
 *          Otherwise SUCCESS 
 *        
"""
def sendEntry(PIgate_ID, Plate):

    now = datetime.now()
    timeNow = now.strftime("%H:%M:%S")
    dateNow = now.strftime("%a %b %d %Y")
    
    dataToSend={
        "PIgate_ID": PIgate_ID,
        "Plate": Plate,
        "Date": dateNow,
        "Time": timeNow
    }

    try:
        db.child("Entry").push(dataToSend)
        return 1
    except:
        return errno.EINVAL
    

"""
 * @brief Receive all the PIgate plates in the database
 * 
 * @return -EINVAL: if and error occurs ;
 *          Otherwise an array with all received plates 
 *        
"""
def getPlates(PIgate_ID):

    plate_array = []

    try:
        plates = db.child("Car_Reg").get()
    except:
        return errno.EINVAL

    for plates in plates.each():
        plate = plates.key()
        aux = plate.split("|")

        if(aux[0] == "Not Available"):
            continue

        if( aux[1] == PIgate_ID):
            plate_array.append(aux[0])

    return plate_array


"""
 * @brief Checks if the isToOpen variavel in ON. If yes, this function
 *  automactly changes that value to OFF
 *
 * @param PIgate_ID 
 * @return -EINVAL: if and error occurs ;
 *          Otherwise the received isToOpen variavel  
 *        
"""
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
        

"""
 * @brief Receive all the existent PIgates in the database
 * 
 * @return -EINVAL: if and error occurs ;
 *          Otherwise an array with all received PIgates 
 *        
"""
def getPIgates():

    PIgate_array = []

    try:
        PIgate_ID = db.child("Gate").get()
    except:
        return errno.EINVAL

    for PIgate_ID in PIgate_ID.each():
        PIgate_array.append( str(PIgate_ID.key()) )

    return PIgate_array
    


            
    




    
