# Excel To RIS to Zotero Converter
Made for one-time use in my lab. My PI had a large excel sheet of 2000+ books, and this script converted them into RIS format for Zotero. Therefore I must emphasize:  
This only works for spreadsheets laid out in the EXACT same way as my PI's spreadsheet, because everything is hard coded. Example here:  
https://docs.google.com/spreadsheets/d/1BM7qBgwt91p0Ws8TPtpLr2uQBmJ_6ogB/edit?usp=sharing&ouid=106243489005294580745&rtpof=true&sd=true  
It needs the input file to be a tsv file.  
Also, although it produces a file in mostly the RIS format, it **is not entirely following all RIS rules.** Specifically, the dates are not formatted at all, even though RIS should always be YYYY/MM/DD/otherInfo. This is because 1) in my spreadsheet the formatting was really inconsistant and interpreting it would have been difficult, and 2) Zotero evidently does not require the dates to be in any format, it will take almost any string (I did have issues with one very long string). Also, RIS has lots of different rules for different formats (book vs. article vs. bulletin etc.) which I am ignoring because it worked fine for importing to Zotero.  
