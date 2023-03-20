const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>   
    <script src=
"https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js">
    </script>
</head>
<body>
    <table id="table">
    </table>
    <p>
        ----
    </p>
    <!--I think the forms tag creates a url link with the values of the input-->
    <div>
        <label for="fname">File Name:</label>
        <input type="text" id="fname" name="fname"><br><br>

        <label for="fname">Path Name:</label>
        <input type="text" id="pname" name="pname"><br><br>

        <label for="fname">File Type Name:</label>
        <input type="text" id="tname" name="tname"><br><br>
        
        <button onclick = "sendFileInfo()">enter</button>
        <select id="option">
            <option value = "1">1</option>
            <option value = "2">2</option>
            <option value = "3">3</option>
        </select>
    </div>
    <p id="GETResult"></p>
    
    <script>
        getList();
        createTable('#table');
        function getList(){
            console.log("attempting to get list");
            var xhttp = new XMLHttpRequest();
            xhttp.onload = function(){
                document.getElementById("GETResult").innerHTML = this.responseText;
                console.log("Request is succesful");
                console.log(list);
            }
            xhttp.open("GET", "/list");
            xhttp.send();

        }
        
        function createTable(selector){
            list = document.getElementById("GETResult").textContent;
            console.log(list);
            console.log("Creating Table");
            var cols = Headers(list, selector); 
  
            // Traversing the JSON data
            for (var i = 0; i < list.length; i++) {
                var row = $('<tr/>');  
                for (var colIndex = 0; colIndex < cols.length; colIndex++)
                {
                    var val = list[i][cols[colIndex]];
                    
                    // If there is any key, which is matching
                    // with the column name
                    if (val == null) val = ""; 
                        row.append($('<td/>').html(val));
                }
                
                // Adding each row to the table
                $(selector).append(row);
            }
        }
        function Headers(selector) {
            var columns = [];
            var header = $('<tr/>');
             
            for (var i = 0; i < list.length; i++) {
                var row = list[i];
                 
                for (var k in row) {
                    if ($.inArray(k, columns) == -1) {
                        columns.push(k);
                         
                        // Creating the header
                        header.append($('<th/>').html(k));
                    }
                }
            }
             
            // Appending the header to the table
            $(selector).append(header);
                return columns;
        }
        function sendFileInfo(){
            fileName = document.getElementById("fname").value;
            pathName = document.getElementById("pname").value;
            typeName = document.getElementById("tname").value;
            option = document.getElementById("option").value;
            sendInfo = {"File Name": fileName, "Path": pathName, "File Type": typeName};
            console.log(JSON.stringify(sendInfo));
            console.log(option);
        }
        function result(){
            
        }
        
    </script>
</body>
</html>
)rawliteral";
