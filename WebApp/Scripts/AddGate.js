import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';

import {getAuth} from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js';

import { getDatabase, ref, set, child, get,push } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js';

const firebaseConfig = {
    apiKey: "AIzaSyB8Dzaeb7D3_QhrrD4gcDuqiSfYmesyIbw",
    authDomain: "pigate-175f8.firebaseapp.com",
    databaseURL: "https://pigate-175f8-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "pigate-175f8",
    storageBucket: "pigate-175f8.appspot.com",
    messagingSenderId: "1027851390443",
    appId: "1:1027851390443:web:50ee2a5f98c75d2a879dae"
};


// Initialize Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth();
const database = getDatabase();

/* VERIFY LOGIN DELAY*/

let delayInMilliseconds = 900;
//Set a 900 mseconds delay so the auth can update
setTimeout(function() {

    if(auth.currentUser == null)
    {
        console.log("User is not logged in");
        window.location.href = "/Login.html";
    }
    else
    {
        console.log("User is logged in");

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


document.getElementById("addGate").onclick = function () {

    let userPIgate_ID = document.getElementById("PIgate_ID").value;
    let password = document.getElementById("password").value;
    let gateName = document.getElementById("gateName").value;


    if(userPIgate_ID === "" || password === "" )
    {
        sendReportMsg("You must fill all the required fields", red);
        return;
    }

    let dbRef = ref(getDatabase());

    get(child(dbRef, `Gate/`)).then((snapshot) => {

        let equal = 0;
        let buffer = snapshot.val();

        for(let index in buffer){
            equal += 1 && ( buffer[index].PIgate_ID == userPIgate_ID );
        }

        if(!equal)
            throw("Error: PIgate doesn't exist");

        if( snapshot.child(`${userPIgate_ID}/Password`).val() != password )
            throw("Error: Invalid Password")


        get(child(dbRef,'Gate_Reg/')).then((snapshot) => {

            let equal = 0;
            let buffer = snapshot.val();

            for(let index in buffer){
                equal += 1 && ( buffer[index].PIgate_ID == userPIgate_ID && buffer[index].email == auth.currentUser.email );
            }

            if(equal)
                throw("Error: PIgate already added");
            
            push(ref(database, 'Gate_Reg/'), {
                PIgate_ID: userPIgate_ID,
                email: auth.currentUser.email,
                description: gateName 
                });

            sendReportMsg("Success: PIgate Added ",green);

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

