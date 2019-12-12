Map is a m x n matrix in a text file (full path to file should be passed to command line).
Columns are separated with a single symbol; rows - with new lines.
Numbers 0-8 represent opened cells without mines.
'*' is a cell that was not opened yet.
'm' is a cell that has mine.
If passed map is inconsistent it will be stated. Otherwise, a new map will be shown in command line.
Some '*' cells will be replaced with 'm', meaning that there is a mine there. Some '*' cells will be replaced with 'E' symbol. This cells are safe to open.
However, there can be map configurations in which decision cannot be made. In that case map without changes will be shown.