// Initialize 
import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';
import {getAuth, signOut } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'
import { getDatabase, ref, remove, child, get, onValue } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js';

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

//Delete in future
console.log(auth);
// --------- End initialization ----------- //

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
        let welcomeString;

        if(auth.currentUser.displayName == null)
        {
            welcomeString = "Welcome !";
        }
        else
        {
            welcomeString = "Welcome " +  auth.currentUser.displayName + " !";
        }
        
        //Get the username and displays it
        const h1 = document.getElementById("mainHeader");
        h1.innerText = welcomeString;


        //Get the user PIgates and display
        const dbRefGateReg = ref(database,'Gate_Reg/');

        onValue(dbRefGateReg, (snapshot) => {
            snapshot.forEach((childSnapshot) => {
                const childKey = childSnapshot.key;
                const childData = childSnapshot.val();

                if( childData.email == auth.currentUser.email )
                {
                    let node = document.createElement("a");
                    let textNode = document.createTextNode(`${childData.description}`);

                    node.appendChild(textNode);
                    node.href = `/LoginSuccess/GateOptions/OptionsMenu.html?id=${childKey}`;
                    document.getElementById("PIgateList").appendChild(node);
                }

            });
          }, {
            onlyOnce: true
          });

        document.body.style.visibility="visible";
    }

  }, delayInMilliseconds);

/* VERIFY LOGIN DELAY*/




document.getElementById("logoutButton").onclick = function() {

    let number;

    const auth = getAuth();
    signOut(auth).then(() => {

        number = 0;
        window.alert("Logout Success");
        window.location.href = "/Login.html";
        
    }).catch((error) => {
        
        number++;
        console.log(number);
        window.alert("Logout Error! Try again")

    });
}
