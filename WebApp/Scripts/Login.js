// Initialize 
import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';

import {getAuth, setPersistence, signInWithEmailAndPassword, browserSessionPersistence  } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'


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
document.body.style.visibility="visible";
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


/* VERIFY LOGIN DELAY*/
let delayInMilliseconds = 900;
//Set a 900 mseconds delay so the auth can update
setTimeout(function() {

    if(auth.currentUser == null)
    {
        console.log("User is not logged in");

    }
    else
    {
        console.log("User is logged in");
        window.location.href = "/LoginSuccess/Gate.html";

    }

  }, delayInMilliseconds);

/* VERIFY LOGIN DELAY*/


// DELETE THIS
console.log(auth);
//
//
document.getElementById("loginButton").onclick = function(){

    let userEmail = document.getElementById("email").value;
    let userPass = document.getElementById("password").value;

    if(userEmail === "" || userPass === "" )
    {
        sendReportMsg("You must fill all the fields", red);
        return;
    }

    setPersistence(auth, browserSessionPersistence)
        .then(() => {

            signInWithEmailAndPassword(auth, userEmail, userPass)
            .then((userCredential) => {
                const user = userCredential.user;
          
                sendReportMsg("Login Success", green);
                window.location.href = "/LoginSuccess/Gate.html";
          
              })
              .catch((error) => {
                const errorCode = error.code;
                const errorMessage = error.message;
                
                switch (errorCode)
                {
                    case "auth/wrong-password": 
                        sendReportMsg("Invalid Password", red);
                        break;
                    
                    default:
                        sendReportMsg("Invalid Email/Password",red);
                        break;
                }
        })
        .catch((error) => {
            // Handle Errors here.
            const errorCode = error.code;
            const errorMessage = error.message;

            errorReport.innerText = "Technical Error! Please try later or contact us!";

        });
      
    });
    
}


