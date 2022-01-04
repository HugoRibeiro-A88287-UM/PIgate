// Initialize 
import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';
import {getAuth, signOut } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'
import { getDatabase, ref, remove, child, get, update, set, push } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js';

const firebaseConfig = {
    apiKey: "AIzaSyB8Dzaeb7D3_QhrrD4gcDuqiSfYmesyIbw",
    authDomain: "pigate-175f8.firebaseapp.com",
    databaseURL: "https://pigate-175f8-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "pigate-175f8",
    storageBucket: "pigate-175f8.appspot.com",
    messagingSenderId: "1027851390443",
    appId: "1:1027851390443:web:50ee2a5f98c75d2a879dae"
};

const app = initializeApp(firebaseConfig);
const auth = getAuth();
const database = getDatabase();
const dbRef = ref(database);

//Controls if it is the first time that the user trying to add a plate
let firstTime = true;

// --------- End initialization ----------- //

/* GET URL PARAMETER FUNCTION */

function getParameterByName(name, url) {
    if (!url) url = window.location.href;
        name = name.replace(/[\[\]]/g, '\\$&');
        var regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'),
            results = regex.exec(url);
    if (!results) return null;
    if (!results[2]) return '';
    return decodeURIComponent(results[2].replace(/\+/g, ' '));
}

const gateRegId = getParameterByName("id");
let gateRegInfo = null; //Gate_Reg object {PIgate_Id, email}
/* GET URL PARAMETER FUNCTION */


/* VERIFY LOGIN DELAY*/

let delayInMilliseconds = 900;
//Set a 900 mseconds delay so the auth can update
setTimeout(function() {

    if(auth.currentUser == null || gateRegId == null)
    {
        console.log("Access Denied");
        window.location.href = "/LoginSuccess/Gate.html";
    }
    else
    {
        console.log("User is logged in");
   
        //Verify if the user is valid for the Gate_Reg Id
        let dbRef = ref(database);

        get(child(dbRef, `Gate_Reg/${gateRegId}`)).then((snapshot) => {
            const obj = snapshot.val();

            if(obj.email == auth.currentUser.email)
            {
                document.body.style.visibility="visible";
                gateRegInfo = obj;
                console.log("Valid user");
            }
            else
            {
                window.location.href = "/LoginSuccess/Gate.html";
            }
        }).catch((error) => {
            console.error(error);
        });

    }

  }, delayInMilliseconds);

/* VERIFY LOGIN DELAY*/

/* REPORT MESSAGE FUNCTION */
const red = 0;
const green = 1;

function sendReportMsg(msg,color){

    const msgReport = document.getElementById("msgReport");

    switch(color)
    {
        case green:
            msgReport.style.color = "rgb(5, 104, 5)";
            break;
        
        case red:
            msgReport.style.color = "rgb(228, 58, 5)";
            break;

        default:
            msgReport.style.color = "black";
            break;
    }

    msgReport.innerText = msg ;
}

/* REPORT MESSAGE FUNCTION */


document.getElementById("goBackButton").onclick = function() {

    window.location.href = `/LoginSuccess/GateOptions/OptionsMenu.html?id=${gateRegId}`;

}


document.getElementById("addButton").onclick = function() {

    let licensePlate = document.getElementById("licensePlateId").value;
    let description = document.getElementById("descriptionId").value;
    let brand = document.getElementById("brandId").value;
    let model = document.getElementById("modelId").value;
    let color = document.getElementById("colorId").value;
    


    if(licensePlate === "" || description === "" )
    {
        sendReportMsg("You must fill all the required fields", red);
        return;
    }


    // Verify Plate format. The available formats are
    //     AA-00-00
    //     00-AA-00
    //     00-00-AA
    //     AA-00-AA
    let vrf1  = (/^[A-Z]{2} ?- ?\d{2} ?- ?\d{2}$/).test(licensePlate);
    let vrf2  = (/^\d{2} ?- ?[A-Z]{2} ?- ?\d{2}$/).test(licensePlate);
    let vrf3  = (/^\d{2} ?- ?\d{2} ?- ?[A-Z]{2}$/).test(licensePlate);
    let vrf4  = (/^[A-Z]{2} ?- ?\d{2} ?- ?[A-Z]{2}$/).test(licensePlate);

    if(!vrf1 && !vrf2 && !vrf3 && !vrf4)
    {
        sendReportMsg("Error: Invalid Plate format",red);
        return;
    }

    get(child(dbRef, `Car/`)).then((snapshot) => {

        let equal = 0;
        let buffer = snapshot.val();

        //Verify if the Car exists on the databse
        for(let index in buffer){
            equal += 1 && ( buffer[index].Plate == licensePlate );
        }

        if(!equal && firstTime)
        {
            firstTime = false;
            document.getElementById("licensePlateId").readOnly = "true";
            document.getElementById("completeInput").style.marginBottom = "auto";
            document.getElementById("completeInput").style.visibility = "visible";
            throw("For a better experience, please insert the following fields");
        }
            

       //Verify if the user has already added to PLate
        get(child(dbRef,'Car_Reg/')).then((snapshot) => {

            let equal = 0;
            let buffer = snapshot.val();

            for(let index in buffer){
                equal += 1 && ( buffer[index].PIgate_ID == gateRegInfo.PIgate_ID && buffer[index].Plate == licensePlate );
            }

            if(equal)
                throw("Error: Plate already added");
            
            //After all verifications, the PIgate is associated with Plate
           
            if(firstTime)
            {
                //The Car is in the system
                push(ref(database, 'Car_Reg/'), {
                    PIgate_ID: gateRegInfo.PIgate_ID,
                    Plate: licensePlate,
                    description: description 
                    });
            }
            else
            {   //The Car is not in the system

                if(brand === "")
                    brand = "Not Availabe";

                if(model === "")
                    model = "Not Availabe";

                if(color === "")
                    color = "Not Availabe";
                
                set(ref(database, `Car/${licensePlate}`), {
                    Plate: licensePlate,
                    Colour: color,
                    Brand: brand,
                    Model: model
                    });
                
                push(ref(database, 'Car_Reg/'), {
                    PIgate_ID: gateRegInfo.PIgate_ID,
                    Plate: licensePlate,
                    description: description 
                    });
            }

            sendReportMsg("Success: PIgate Added ",green);

            setTimeout( () => {location.reload();}, 1000);

        }).catch((error) => 
        {
            console.error(error);
            sendReportMsg(error,red);
        });

    }).catch((error) => {
        sendReportMsg(error,red);
        console.error(error);

    });

}

