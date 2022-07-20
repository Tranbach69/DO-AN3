//const axios =require("axios").default

let buttonDelete = document.querySelector('.ButtonDelete')
let id = document.querySelector('#input200')

buttonDelete.onclick = function () {
    deleteById(id.value);
}

const deleteById = async (id) =>{
    try{
        const response = await axios.delete(`https://bachtran69.herokuapp.com/api/patient/${id}`);
        console.log(response.data);
    }catch(error){
        console.log("error",error);
    }
};
const loadAll = async (id) =>{
    try{
        var response = await axios.get(`https://bachtran69.herokuapp.com/api/patient/${id}`);
        console.log(response.data);
        var object =JSON.parse(response.data);
        console.log(object);
    }catch(error){
        console.log("error",error);
    }
};
var x=loadAll(1);
//var object =JSON.parse('{"name":"John", "age":30, "city":"New York"}');

