// Initialize Firebase
import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';

import {getAuth, createUserWithEmailAndPassword, updateProfile, signOut} from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'

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

// --------- End initialization ----------- //

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

document.getElementById("registerButton").onclick = function() {

    let userEmail = document.getElementById("email").value;
    let userPass = document.getElementById("password").value;
    let username = document.getElementById("username").value;
    //let userphone = document.getElementById("phone").value;


    if(userEmail === "" || userPass === "" || username === "" )
    {
        sendReportMsg("You must fill all the required fields", red);
        return;
    }

    createUserWithEmailAndPassword(auth, userEmail, userPass)
        .then(() => {
            // Signed in 
            const user = auth.currentUser;

            updateProfile(user,{
                displayName: username,
            });
            
            //Firebase logs in automatically in the user account when it is created
            //So it's necessary to logout to obey the design
            signOut(auth).then(() => {
                
            }).catch((error) => {
                //Do nothing because the account is already created
            });

            sendReportMsg("New account created!", green);

        })
        .catch((error) => {
            const errorCode = error.code;
            const errorMessage = error.message;

            switch(errorCode)
            {
                case "auth/invalid-email":
                    sendReportMsg("Invalid Email", red);
                    break;

                case "auth/email-already-in-use":
                    sendReportMsg("Email already in use",red);
                    break;

                case "auth/weak-password":
                    sendReportMsg("Weak Password", red);
                    break;

                default:
                    sendReportMsg("Error in register. Try later", red)
                    break;

            }

            console.log(errorCode);

        });

    return 1;
}