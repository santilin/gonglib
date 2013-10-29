/*<<<<<COPYRIGHT*/
/** @file rtk.cpp Includes all the RTK functionality */

/** \mainpage

  This is the RTK (Reports Tool Kit), a report generator tool.

  It is intended to build reports from any input source and to any output destination.

  The layout of the report is defined in an XML file.

  Let's see how to create a sample report to show the users in your /etc/passwd file. This sample report is located in samples/system/passwd.rtk in the RTK distribution.

  \b Creating \b the \b XML \b template

  Create a file with your favourite text editor and name it \p passwd.rtk. Type in the following lines

\verbatim
<?xml version="1.0" encoding = "iso-8859-1"?>
<!DOCTYPE REPORT [
    <!ENTITY rtkstyles SYSTEM "rtkstyles.xml">
    ]
>
\endverbatim

	This declares this file as an XML document with Western encoding and sets the location of some basic styles for the layout of the report.

	Now, start the report definition, setting its name, title, units and sizes.

\verbatim
<REPORT name="lopd.rtk" title="Users in your system"
	units="chars" sizex="80" sizey="60">
\endverbatim

	To effectively include the styles declared above, we have to include the external entity using the XML syntax. Then, we redefine the style for the integer quantities in order to show leading zeros.

\verbatim
&rtkstyles;

<STYLE name="integer"  format="000000" halignment="right"/>
\endverbatim


  \b Defining \b the \b Input.

  With Reports, you can define several different input sources for the same report. Our report will use the file /etc/passwd, which is a "colon separated values" file.

\verbatim
\endverbatim

	We have declared 6 fields. Each field consists of a name, the source (in this case the column number) and the type. Later on, the field \a user will be referred to as \a input.user.

	In this way, we could define another input with the same field names but different sources, and we still refer to the user field as \a input.user throught the report.

\verbatim
<INPUT name="sql-input" driver="sql"
		sqldriver="QMYSQL3" database="mydatabase" user="users" password=""
		from="mytabale"
		orderby="partida" >
    <INPUTFIELD name="user" source="mytable.user" type="text"></INPUTFIELD>
    <INPUTFIELD name="uid" source="mytable.uid" type="integer"></INPUTFIELD>
    <INPUTFIELD name="gid" source="mytable.gid" type="integer"></INPUTFIELD>
    <INPUTFIELD name="realname" source="mytable.realname" type="text"></INPUTFIELD>
    <INPUTFIELD name="homedir" source="mytable.header" type="text"></INPUTFIELD>
    <INPUTFIELD name="shell" source="mytable.shell" type="text"></INPUTFIELD>
</INPUT>
\endverbatim

  \b Defining \b the \b report \b layout

  The report layout is structured into sections. There are seven types of sections: Report Header, Report Footer, Page Header, Page Footer, Group Header, Group Footer and Details. All of them can contain any number of objects like text, lines, images, etc.

  You can have more than one section of any of those types, and they will be laid out in the order in which they are defined in the template. To differentiate them, you can use the \a name property.

  We will start with the Page Header section. It will contain the heading of the report. The type must be \a page-header.

\verbatim
  <SECTION name="pageheader" type="page-header" style="header" sizey="2" sizex="80">
\endverbatim

	The style states how all the objects in this section should be formatted. Each object can define their own style later on, which would override this style setting. The \a sizey and \a sizex are considered to be in \p chars, as that is the unit we have defined for the whole report.

\verbatim
    <OBJECT name="title" style="title"
      posx="0" posy="0" sizex="80" sizey="1"
	  adjustment="grow"
      value="Users in your system"
      ></OBJECT>
</SECTION>
\endverbatim

	We have added one object to this section. We have defined its name, position, size and value.
	If we don't want to bother about the width of a field, we just set \a sizex=-1 or supress it.
	In the \a titulo object we have set the adjustment to \p grow, which means that the field can grow in heigth if it does not fit in its space.
	The value contains a constant string, so this field is constant and will be calculated only once.

	The next section is another Page Header section. It will contain the Headings of the columns. We have created a different Page Header section because we want the background color to be different from the previous one.

\verbatim
<SECTION name="pageheader-2" type="page-header" sizey="1" sizex="80" backcolor="grey">
    <OBJECT name="header-uid"
      posx="0" posy="0" sizey="1"
      value="Id"
      ></OBJECT>
    <OBJECT name="header-user"
      posx="7" posy="0" sizey="1"
      value="User"
      ></OBJECT>
    <OBJECT name="header-gid"
      posx="24" posy="0" sizey="1"
      value="Group"
      ></OBJECT>
    <OBJECT name="header-realname"
      posx="32" posy="0" sizey="1"
      value="Name"
      ></OBJECT>
    <OBJECT name="header-homedir"
      posx="50" posy="0" sizey="1"
      value="Home Dir"
      ></OBJECT>
    <OBJECT name="header-shell"
      posx="68" posy="0" sizey="1"
      value="Shell"
      ></OBJECT>
</SECTION>
\endverbatim

	The next section is the details one. It will be printed for every record or line in the \a Input. The type must be \p details and, as above, there can be more than one details section per report. We use a different style for this section.
	The level says the depth level of this section in the report. It is used to maintain groups together. In this case, as we do not have groups, the level is one.


\verbatim
<SECTION name="details" type="details" level="1" style="details" sizey="1" sizex="80">
    <OBJECT name="uid"
      posx="0" posy="0" sizex="6" sizey="1"
      value="input.uid"
      ></OBJECT>
    <OBJECT name="user"
      posx="7" posy="0" sizex="17" sizey="1"
      value="input.user"
      ></OBJECT>
    <OBJECT name="gid"
      posx="25" posy="0" sizex="6" sizey="1"
      value="input.gid" format="00000"
      ></OBJECT>
    <OBJECT name="realname"
      posx="32" posy="0" sizex="7" sizey="1"
	  adjustment="grow" fontcolor="red"
      value="input.realname"
      ></OBJECT>
    <OBJECT name="homedir"
      posx="50" posy="0" sizex="17" sizey="1"
      value="input.homedir"
      ></OBJECT>
    <OBJECT name="shell"
      posx="68" posy="0"  sizey="1"
      value="input.shell"
      ></OBJECT>
</SECTION>
\endverbatim

	The objects are defined in the same way that in the Page Header section. Now the value is not constant, it is an expression that returns the value of a field in the \a Input. For example, \a value="input.user" refers to the INPUTFIELD named "user" in the INPUT element of this template. We have changed the format of the \a gid field to show leading zeros and the adjustment and text color of the \a realname field. If this field doesn't fit in its space, it will expand over the following lines.

	The last two sections are the Page Footers. We will add a first Page Footer with some totals.

\verbatim
<SECTION name="pagefooter" type="page-footer" posy="0" sizey="1" sizex="80" backcolor="gray">
    <OBJECT name="total-text"
      posx="0" posy="0" sizex="23" sizey="1"
      value="Totals"
      ></OBJECT>
    <OBJECT name="total-uid"
      posx="24" posy="0" sizex="7" sizey="1"
	  aggregate="sum" style="integer"
      value="input.uid"
      ></OBJECT>
    <OBJECT name="avg-gid"
      posx="38" posy="0" sizex="12" sizey="1"
	  aggregate="avg" style="decimal"
      value="input.gid" format="###,###,##0.00"
      ></OBJECT>
</SECTION>
\endverbatim

	The field named \p total-uid is defined as the sum of the value \a input.uid.
	The field named \p avg-uid is defined as the average of the value \a input.gid. It uses the style \p decimal, defined in the standard rtkstyles.xml file.
	The other Page Footer section will print the number of records and the page number.

\verbatim
<SECTION name="pagefooter-2" type="page-footer" posy="0" sizey="1" sizex="80" style="footer-text">
      <OBJECT name="nrecords-text" value="Number of users:" posx="0" sizey="1"></OBJECT>
      <OBJECT name="nrecords" aggregate="count" value="recordcount()" style="integer"
	  posx="16" posy="0" sizex="20" sizey="1"></OBJECT>
      <OBJECT name="npagina-text" value="Page:" posx="70" sizex="7" sizey="1"></OBJECT>
      <OBJECT name="npagina" value="pagenumber()" posx="77" sizex="3" sizey="1"></OBJECT>
</SECTION>
\endverbatim

	And that's all. We only need to close the report definition and save the file.

\verbatim
</REPORT>
\endverbatim


\b Viewing \b the \b report.

	To view the report on the X Window, just type:

\verbatim
	rtkview passwd.rtk
\endverbatim

	And you'll come up with a nice window and all your users listed.
	If the styles can not be found, you can set the path

\verbatim
	rtkview --styles-path=/path/to/styles passwd.rtk
\endverbatim

	You can not only view the report, but also print it to different formats:

\verbatim
	rtkview --format html --output passwd.html passwd.rtk

	rtkview --format text --output passwd.txt passwd.rtk

	rtkview --format ODFCalc --oo-template-dir=/path/to/opendocument_template --output users.sxc passwd.rtk
\endverbatim

	To view all the different arguments of rtkview, type

\verbatim
	rtkview --help
\endverbatim


*/

