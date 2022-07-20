let tableUser = document.querySelector(".tbody");

var loadAllUsers = async () => {
  try {
    let html = "";
    var response = await axios.get(
      `https://bachtran69.herokuapp.com/api/patient`
    );
    response.data.map((arr) => {
      html += ` 
        <tr>
            <td>${arr.id}</td> 
            <td>${arr.Name}</td> 
            <td>${arr.Age}</td>
            <td>${arr.Address}</td>
            <td>${arr.BodyTemperature}</td>
            <td>${arr.Condition} </td>
            
        </tr>       
       `;
    });
    console.log(html);
    tableUser.innerHTML = html;
    // var object =JSON.parse(response.data);
    // console.log(object);
  } catch (error) {
    console.log("error", error);
  }
};
loadAllUsers();
