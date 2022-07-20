let email = document.querySelector('#normal_login_email')
let pass = document.querySelector('#normal_login_password')
let btnLogin = document.querySelector(".login-form-button")

btnLogin.onclick = () => {
    getUsers()
        .then(rs => {
            let email = getEmail()
            let pass = getPassword()

            let result = rs.some(arr => {
                return email == arr.email && pass == arr.password
            })
            if(result){
                alert("Dang nhap thanh cong")
                window.location= `https://www.facebook.com`
            }
            else{
                alert("Dang nhap that bai")
            }
        })

}

async function getUsers() {
    let data = await fetch("https://bachtran69.herokuapp.com/api/patient")
        .then(data => {
            return data.json()
        })
    return data
}
function getEmail() {
    return email.value
}
function getPassword() {
    return pass.value
}















