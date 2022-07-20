let btnShow = document.querySelector('.btn__show');
let table = document.querySelector('.table__user')
let i=0;
btnShow.onclick = () => {
    if(i%2 ==0){
        table.classList.remove('d-none')
        btnShow.innerHTML = `Hide`
    i++
    }
    else{
        table.classList.add('d-none')
        btnShow.innerHTML = `Show`
        i++
    }
}