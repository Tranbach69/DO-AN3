
let addName = document.querySelector('#addNamePatient')
let addAge = document.querySelector('#addAgePatient')
let addPhone = document.querySelector('#addPhonePatient')
let addAddress = document.querySelector("#addAddressPatient")
let btnAdd = document.querySelector(".ButtonAdd")

btnAdd.onclick = function () {
    alert("d");
    let Name = addName.value
    let Age = addAddress.value
    let Phone = addPhone.value
    let Address = addAddress.value
    const data = {
        Name,
        Age,
        Phone,
        Address,
        "BodyTemperature" : " ",
        "Condition":" "
    }
    axios.post('https://bachtran69.herokuapp.com/api/patient', data)
        .then(function (response) {
            console.log(response);
            alert("a")
        })
        .catch(function (error) {
            console.log(error);
        });
    //deleteById(5);
}
