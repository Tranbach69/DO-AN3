let editName = document.querySelector('#editNamePatient')
let editAge = document.querySelector('#editAgePatient')
let editPhone = document.querySelector('#editPhonePatient')
let editAddress = document.querySelector("#editAddressPatient")
let btnEdit = document.querySelector(".ButtonEdit")
let idPatient = document.querySelector("#idPatient")

btnEdit.onclick = function () {
    alert("a");
    let Name = editName.value
    let Age = editAge.value
    let Phone = editPhone.value
    let Address = editAddress.value
    let idPatient1=idPatient.value
    const data = {
        // Name,
        // Age,
        // Phone,
        Address
    }
    axios.patch(`https://bachtran69.herokuapp.com/api/patient/${idPatient1}`,data)
        .then(function (response) {
            console.log(response);
            alert("a")
        })
        .catch(function (error) {
            console.log(error);
        });
}





