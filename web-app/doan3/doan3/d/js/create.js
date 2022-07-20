
let email = document.querySelector('#register_email')
let pass = document.querySelector('#register_password')
let phone = document.querySelector('#register_phonenumber')
let userName = document.querySelector("#register_username")
let btnCreate = document.querySelector(".btn__create")

btnCreate.onclick = function () {

    let email = getEmail()
    let password = getPassword()
    let phone1 = phone.value
    let userName1 = userName.value
    const data = {
        email,
        password,
        phone1,
        userName1
    }
    axios.post('https://bachtran69.herokuapp.com/api/patient', data)
        .then(function (response) {
            console.log(response);
        })
        .catch(function (error) {
            console.log(error);
        });
    //deleteById(5);
}
function getEmail() {
    return email.value
}
function getPassword() {
    return pass.value
}

// const deleteById = async (id) =>{
//     try{
//         const response = await axios.delete(`https://bachtran69.herokuapp.com/api/patient/${id}`);
//         console.log(response.data);
//     }catch(error){
//         console.log("error",error);
//     }
// };
let buttonDelete = document.querySelector('.ButtonDelete')
let id = document.querySelector('#input200')

buttonDelete.onclick = function () {

    deleteById(4);
    alert('aa');
}