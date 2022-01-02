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


// Initialize Firebase
const app = initializeApp(firebaseConfig);

const auth = getAuth();

console.log(auth);
if(auth.currentUser == null)
{
    console.log("User is not logged in");
}
else
{
    console.log("User is logged in");
    //window.location.href = "/LoginSuccess/Gate.html";
}


document.getElementById("loginButton").onclick = function(){

    var userEmail = document.getElementById("email").value;
    var userPass = document.getElementById("password").value;

    setPersistence(auth, browserSessionPersistence)
        .then(() => {

            console.log("OK");

            signInWithEmailAndPassword(auth, userEmail, userPass)
            .then((userCredential) => {
                const user = userCredential.user;
          
                window.location.href = "/LoginSuccess/Gate.html";
          
              })
              .catch((error) => {
                const errorCode = error.code;
                const errorMessage = error.message;
                
                switch (errorCode)
                {
                    case "auth/wrong-password": 
                        window.alert("Invalid Password");
                        break;
                    
                    default:
                        window.alert("Invalid Email/Password");
                        break;
                }
        })
        .catch((error) => {
            // Handle Errors here.
            const errorCode = error.code;
            const errorMessage = error.message;

            console.log("Erro in persistence")

        });
      
    });
    
}


