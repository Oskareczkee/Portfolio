//my implementation of game i written for test for someone in college
var tableCols : number =9;
var tableRows: number =11 +1;
var dropMin : number =1;
var dropMax : number =3;
var buildingsNum: number =tableCols;

var invaderSpawnChance :number =0.1;
var missileRechargeTurns :number=2;

let $gameTable= $("#gameTable");
let invadersCoords : Array<{row: number, col: number}>=[]
let missileUsageTurns : Array<number>=[];

var gameEnded : boolean=false;

var PewPew = new Audio("img/pewpew.mp3");
PewPew.volume=0.2;
var InvaderDestroy = new Audio("img/invaderSound.mp3");
InvaderDestroy.volume=0.2;
var FailureSound = new Audio("img/failure.mp3");
FailureSound.volume=0.2;
var SucessSound = new Audio("img/success.mp3");
SucessSound.volume=0.2;



function generateTable(rows :number , cols: number)
{
    buildingsNum=tableCols;
    gameEnded=false;
    //0 is start row tableRows + 1 is the last one according to the documentation
    for(var x =0; x<=rows;x++)
    {
        let row = $("<tr></tr>").appendTo($gameTable);

        for(var y=0; y<=cols;y++)
        {
            let cell = $("<td></td>").appendTo(row);
            if(x==0 && y!=0)
            {
                cell.addClass("package");
                invadersCoords.push({row: x, col: y});
            }
            if(y==0 && x!=0 && x!=tableRows)
            {
                cell.addClass("missile");
                cell.attr('onclick', 'shootMissile(' + x + ')');
                missileUsageTurns.push(0);
            }

            if(y!=0 && x==rows)
                cell.addClass("building");
        }
    }
}generateTable(tableRows, tableCols);

function dropInvaders()
{
    var invaderToDestroy:  Array<{row: number, col: number}>=[];
    for(var x =0 ; x<invadersCoords.length;x++)
    {
        let invaderCoords = invadersCoords[x];
        let dropRate = getRandomInt(dropMin, dropMax);
        var nextPos = invaderCoords.row + dropRate;

        $gameTable.find('tr')[invaderCoords.row].cells[invaderCoords.col].classList.remove("package");
        //invader has hit the building, do not update him, we will destroy them later
        if(invaderCoords.row + dropRate >= tableRows)
        {
            invaderToDestroy.push(invaderCoords);
            $gameTable.find('tr')[tableRows].cells[invaderCoords.col].classList.add("building-destroyed");
            buildingsNum--;
            continue;
        }

        var $invaderNextPos = $gameTable.find('tr')[nextPos].cells[invaderCoords.col];

        //invader has hit another invader - do nothing
        if($invaderNextPos.classList.contains("package"))
            continue;

        //if invader has not been yet destroyed, update him
        $invaderNextPos.classList.add("package");
        invadersCoords[x].row+=dropRate;
    }

    if(invaderToDestroy.length)
        InvaderDestroy.play();

    for(var x =0 ; x<invaderToDestroy.length;x++)
    {
        var item  = invadersCoords.filter(i => i.row===invaderToDestroy[x].row && i.col===invaderToDestroy[x].col)[0];
        var index  = invadersCoords.indexOf(item);
        invadersCoords.splice(index,1);
    }

    if(checkinvaderWin())
    {
        FailureSound.play();
        alert("Invaders won!");
        gameEnded=true;
    }
}dropInvaders();

function spawnInvaders()
{
    for(var x=1; x<=tableCols;x++)
    {
        //if roll is true, spawn invader
        var roll :boolean = Math.random() <= invaderSpawnChance ? true : false;
        if(roll)
        {
            //check if there is no invader yet, if there is, do not spawn new one
            var $col =$gameTable.find('tr')[0].cells[x];
            if(!$col.classList.contains("package"))
            {
                $col.classList.add("package");
                invadersCoords.push({row: 0, col: x});
            }
        }
    }
}

function getRandomInt(min : number, max: number) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1) + min);
}

function restartTable()
{
    while(invadersCoords.length)
        invadersCoords.pop();
    $gameTable.html('');
    generateTable(tableRows, tableCols);
    dropInvaders();
}

function checkinvaderWin(): boolean
{
    if(buildingsNum<=0)
        return true;
    return false;
}

function recoverMissiles()
{
    for(var x =0 ; x< missileUsageTurns.length;x++)
    {
        if(missileUsageTurns[x]===0)
            continue;

        missileUsageTurns[x]--;

        //if missile became usable, recover it
        if(missileUsageTurns[x]===0)
            $gameTable.find('tr')[x+1].cells[0].classList.remove('used');
    }
}

function shootMissile(row : number)
{
    if(gameEnded)
        return;

    var $row =$gameTable.find('tr')[row];

    //missile has been used, return and do nothing
    if($row.cells[0].classList.contains("used"))
        return;

    //when we know, that we can shoot, first recover missiles, otherwise missile we shoot will have cooldown -1 instantly
    recoverMissiles();

    for(var x = 1; x<=tableCols;x++)
    {
        var $col =$row.cells[x];
        if($col.classList.contains("package"))
        {
            $col.classList.remove("package");
            var item  = invadersCoords.filter(i => i.row===row && i.col===x)[0];
            var index  = invadersCoords.indexOf(item);
            invadersCoords.splice(index,1);    
        }
    }

    //if this value is 0, missiles should never need to recharge, this fixes small bug for value 0 of missileRechargeTurns
    if(missileRechargeTurns)
        $row.cells[0].classList.add("used");

    missileUsageTurns[row-1]+=missileRechargeTurns;

    PewPew.play();
    
    if(invadersCoords.length===0)
    {
        SucessSound.play();
        alert("Congratulations, you won !");
        gameEnded=true;
        return;
    }
    dropInvaders();
    spawnInvaders();

    console.log(buildingsNum);
    console.log(gameEnded);
}

function skipTurn()
{
    if(gameEnded)
        return;

    dropInvaders();
    spawnInvaders();
    recoverMissiles();
}

function applySettings()
{
    var dropMinVal =Number($("#package-drop-min").val());
    var dropMaxVal =Number($("#package-drop-max").val())
    var tableColsVal =Number($("#col-num").val());
    var tableRowsVal = Number($("#row-num").val());
    
    if(dropMinVal > dropMaxVal)
    {
        alert("Drop min cannot be greater than drop max !");
        return;
    }
    
    dropMin=dropMinVal;
    dropMax=dropMaxVal;
    if(tableColsVal)
        tableCols = tableColsVal;
    if(tableRowsVal)
        tableRows=tableRowsVal;
    invaderSpawnChance=0.01*Number($("#invader-spawn-chance").val());
    //convert to percents
    missileRechargeTurns=Number($("#missile-recharge-turns").val());

    restartTable();
}