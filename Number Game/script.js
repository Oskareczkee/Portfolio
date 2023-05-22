var rows =7;
var cols =5;

var move_count=0;
var start=1;

var table = document.getElementById("gameTable");
var values=[]

function generateTable(rows, cols)
{
    //clear the table
    table.innerHTML='';
    generateValues();
    var counter=0;
    var zeroCoords={row: 0, col: 0};

    for(x =0; x<rows;x++)
    {
        let row = table.insertRow(x);

        for(y=0; y<cols;y++)
        {
            var cell = row.insertCell(y);
            
            cell.innerHTML=values[counter];
            if(values[counter]===0)
            {
                cell.classList.add("zero");
                zeroCoords={row: x, col: y};
            }

            cell.addEventListener("click", cellClick.bind(null, {row: x, col: y}));

            counter++;
        }
    }
    checkOrder();
    update_zero_neighbors(zeroCoords);
}generateTable(rows, cols);

function generateValues()
{
    //erase values before generation
    while(values.length)
        values.pop();

    values.push(0);
    //generujemy nowe
    for(var x=start;x<=(cols*rows);x++)
        values.push(x);

    //make values in random order
    values = values.sort((a,b)=>0.5-Math.random());
}

function newGame()
{
    generateTable(rows, cols);
    move_count=0;
    update_move_count();
}

function swapCell(cell, c)
{
    var copy = cell.innerHTML;
    cell.innerHTML=c.innerHTML;
    cell.classList.add("zero");
    c.innerHTML=copy;
    c.classList.remove("zero");
}


function cellClick(coords)
{
    var cell = table.rows[coords.row].cells[coords.col];
    var newZeroCoords={row: 0, col:0};
    var oldZeroCoords={row: 0, col:0};

    if(coords.row+1<rows)
    {
        var c = table.rows[coords.row+1].cells[coords.col];
        if(c.innerHTML==="0")
        {
            swapCell(cell, c);
            oldZeroCoords={row: coords.row+1, col: coords.col};
            newZeroCoords=coords;
            move_count++;
            update(oldZeroCoords, newZeroCoords);
            return;
        }
    }

    if(coords.row-1>=0)
    {
        var c = table.rows[coords.row-1].cells[coords.col];
        if(c.innerHTML==="0")
        {
            swapCell(cell, c);
            oldZeroCoords={row: coords.row-1, col: coords.col};
            newZeroCoords=coords;
            move_count++;
            update(oldZeroCoords, newZeroCoords);
            return;
        }
    }
    
    if(coords.col+1<cols)
    {
        var c = table.rows[coords.row].cells[coords.col+1];
        if(c.innerHTML==="0")
        {
            swapCell(cell, c);
            oldZeroCoords={row: coords.row, col: coords.col+1};
            newZeroCoords=coords;
            move_count++;
            update(oldZeroCoords, newZeroCoords);
            return;
        }
    }
    
    if(coords.col-1>=0)
    {
        var c = table.rows[coords.row].cells[coords.col-1];
        if(c.innerHTML==="0")
        {
            swapCell(cell, c);
            oldZeroCoords={row: coords.row, col: coords.col-1};
            newZeroCoords=coords;
            move_count++;
            update(oldZeroCoords, newZeroCoords);
            return;
        }
    }
}

function update(oldZeroCoords, newZeroCoords)
{
    if(checkWin())
    {
        alert("Congratulations, you won!");
        generateTable(rows, cols);
        move_count=0;
        update_move_count();
        return;
    }

    console.log(oldZeroCoords, newZeroCoords);
    
    update_move_count();
    checkOrder();
    delete_zero_neighbors(oldZeroCoords);
    update_zero_neighbors(newZeroCoords);
}

function delete_zero_neighbors(zeroCoords)
{
    if(zeroCoords.row+1<rows)
        table.rows[zeroCoords.row+1].cells[zeroCoords.col].classList.remove("draggable");
    if(zeroCoords.row-1>=0)
        table.rows[zeroCoords.row-1].cells[zeroCoords.col].classList.remove("draggable");
    if(zeroCoords.col+1<cols)
        table.rows[zeroCoords.row].cells[zeroCoords.col+1].classList.remove("draggable");
    if(zeroCoords.col-1>=0)
        table.rows[zeroCoords.row].cells[zeroCoords.col-1].classList.remove("draggable");
}

function update_zero_neighbors(zeroCoords)
{
    if(zeroCoords.row+1<rows)
        if(!table.rows[zeroCoords.row+1].cells[zeroCoords.col].classList.contains("well-positioned"))
            table.rows[zeroCoords.row+1].cells[zeroCoords.col].classList.add("draggable");

    if(zeroCoords.row-1>=0)
        if(!table.rows[zeroCoords.row-1].cells[zeroCoords.col].classList.contains("well-positioned"))
            table.rows[zeroCoords.row-1].cells[zeroCoords.col].classList.add("draggable");
            
    if(zeroCoords.col+1<cols)
        if(!table.rows[zeroCoords.row].cells[zeroCoords.col+1].classList.contains("well-positioned"))
            table.rows[zeroCoords.row].cells[zeroCoords.col+1].classList.add("draggable");

    if(zeroCoords.col-1>=0)
        if(!table.rows[zeroCoords.row].cells[zeroCoords.col-1].classList.contains("well-positioned"))
        table.rows[zeroCoords.row].cells[zeroCoords.col-1].classList.add("draggable");
}

function update_move_count()
{
    var a = document.getElementById("move-count");
    a.innerHTML=move_count;
}

function checkWin()
{
    var val=start;
    for(var x = 0;x<rows;x++)
    {
        for(var y =0;y<cols;y++)
        {
            var value = parseInt(table.rows[x].cells[y].innerHTML);
            if(value===val || value===0)
            {
                val++;
                continue;
            }
            else
            return false;
        }
    }
    return true;
}

function checkOrder()
{
    var val=start;
    for(var x = 0;x<rows;x++)
    {
        for(var y =0;y<cols;y++)
        {
            var value = parseInt(table.rows[x].cells[y].innerHTML);
            if(value===val)
                table.rows[x].cells[y].classList.add("well-positioned");
            else
                table.rows[x].cells[y].classList.remove("well-positioned");

            val++;

        }
    }
}

function applySettings()
{
    var tableColsVal = document.getElementById("col-num").value;
    var tableRowsVal = document.getElementById("row-num").value;

    if(tableColsVal)
        cols = tableColsVal;
    if(tableRowsVal)
        rows = tableRowsVal;

    generateTable(rows, cols);
}