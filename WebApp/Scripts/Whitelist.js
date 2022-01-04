// Initialize 
import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';
import {getAuth, signOut } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'
import { getDatabase, ref, remove, child, get, update, set, push, onValue } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js';

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
const dbRefCarReg = ref(database,'Car_Reg/');

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

            if(obj.email != auth.currentUser.email)
                throw("Invalid User");

            //Get the user whitelist and display
            onValue(dbRefCarReg, (snapshot) => {
                snapshot.forEach((childSnapshot) => {
                    const childKey = childSnapshot.key;
                    const childData = childSnapshot.val();

                    if( childData.PIgate_ID == gateRegInfo.PIgate_ID)
                    {
                        let node = document.createElement("a");
                        let textNode = document.createTextNode(`${childData.Plate} | ${childData.Description}`);

                        node.appendChild(textNode);
                        document.getElementById("whitelist").appendChild(node);
                    }

                });
            }, {
                onlyOnce: true
            });

            document.body.style.visibility="visible";
            gateRegInfo = obj;
            console.log("Valid user");

        }).catch((error) => {
            console.error(error);
            window.location.href = "/LoginSuccess/Gate.html";
        });

    }

  }, delayInMilliseconds);

/* VERIFY LOGIN DELAY*/

document.getElementById("goBackButton").onclick = function() {

    window.location.href = `/LoginSuccess/GateOptions/OptionsMenu.html?id=${gateRegId}`;

}