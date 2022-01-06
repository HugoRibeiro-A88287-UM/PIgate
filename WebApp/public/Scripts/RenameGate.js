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
            msgReport.style.color = "rgb(80, 247, 94);";
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


document.getElementById("renameButton").onclick = function() {

    let newName = document.getElementById("newName").value;

    if(newName === "")
    {
        sendReportMsg("You must fill all the required fields", red);
        return;
    }

    try 
    {
        update(ref(database, `Gate_Reg/${gateRegId}`), {
            description: newName
            });

        
        sendReportMsg("Success: Gate was renamed", green);
    } 
    catch (e) 
    {
        sendReportMsg(e, red);
    }
 
}


//This functions triggers when the enter key is pressed
document.getElementById("newName").addEventListener("keyup", function(event) {
    event.preventDefault();

    //Enter Key = 13
    if (event.keyCode === 13) {
        document.getElementById("renameButton").click();
    }
});

