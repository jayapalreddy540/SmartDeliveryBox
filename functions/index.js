const functions = require("firebase-functions");
const admin = require("firebase-admin");
admin.initializeApp();

const twilio = require("twilio");

const accountSid = "ACe879810295cd78f65cd3b2d943e88054";
const authToken = "131f96e3f2f290683a7b68ebabe7026f";

const client = new twilio(accountSid, authToken);

const twilioNumber = "+17346660338"; // your twilio phone number

/// start cloud function

exports.sendOTP = functions.database
  .ref("/users/{uid}/otp")
  .onUpdate((change, context) => {
    console.log(context.params.uid);
    const uid = context.params.uid;

    return admin
      .database()
      .ref(`/users/${uid}`)
      .once("value")
      .then(snapshot => snapshot.val())
      .then(order => {
        const phoneNumber = order.mobile;
        const otp = order.otp;
        if (!validE164(phoneNumber)) {
          throw new Error("number must be E164 format!");
        }

        const textMessage = {
          body: `Smart Delivery Box OTP : ${otp}`,
          to: phoneNumber, // Text to this number
          from: twilioNumber // From a valid Twilio number
        };

        return client.messages.create(textMessage);
      })
      .then(message => console.log(message.sid, "success"))
      .catch(err => console.log(err));
  });

/// Validate E164 format
function validE164(num) {
  return /^\+?[1-9]\d{1,14}$/.test(num);
}
