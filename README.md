And now, ladies and gentlemen, we present you with the ultimate hardware accessory from RIKAFR SYSTEMS,

### The Mitsotakiera

> What is this?

A Marafeti In The Middle that sits between your keyboard and computer and intercepts keystrokes, inserting obligatory completions wherever they are most appropriate.

> How?

Well, there's are these two Raspberry PICOs, one called Takis and acting as a USB host intercepting HID reports from the keyboard and the other one, Mitsos, acting as a USB device, sending the intercepted reports to the target. The two PICOs are connected through a UART and communicate using a XML-based prot.......

> Wait.

What?

> You said XML.

Yes.

> You use XML in a MICROCONTROLLER project?

Of course. Because PICO SDK did not include the sscanf() function I was going to use for deserializing messages on the Mitsos side, so I went for the next rational choice.

> And you call XML a "rational choice"!?

It is. Because the design goals of XML emphasize simplicity, generality, and usability across the Internet. It is a textual data format with strong support via Unicode for different human languages. Although the design of XML focuses on documents, the language is widely used for the representation of arbitrary data structures such as those used in web services. The main purpose of XML is serialization, i.e. storing, transmitting, and reconstructing arbitrary data. For two disparate systems to exchange information, they need to agree upon a file format. XML standardizes this process. XML is analogous to a lingua franca for representing information. it has come into common use for the interchange of data over the Internet. Hundreds of document formats using XML syntax have been developed, including RSS, Atom, Office Open XML, OpenDocument, SVG, and XHTML. XML has also provides the base language for communication protocols such as SOAP and XMPP. It is the message exchange format for the Asynchronous JavaScript and XM....

> FOR GODS SAKE, YOU USE XML IN A MICROCONTROLLER?

Well, it not that "micro", technically. In my age, microcontrollers....

> FOR GODS SAKE AGAIN, YOU USE XML IN A MICROCONTROLLER?

You officially run a full fucking Python interpreter on that microcontroller in order for 5 year olds and post-millenial hipsters to be able to use it. And then they think they can do things like signal processing and real time scheduling in a fucking interpreted language. And then the 5 year olds grow up and take hipster jobs in the industry. And then they design actual hipster systems while only knowing Python. And then people die because the embedded Python core of a pacemaker some hipster programmed decided to garbage collect during a cardiac arrest. A woman in Metsovo died when her Python controlled airplane landing system... AND YET YOU DARE HAVING THE AUDACITY TO COMPLAIN ABOUT XML?

> err.. well, yes, ok, you have a point there, but the python thing, we, we... WE DO IT FOR THE EDUCATION!

So do I. XML is an excellent educational tool

> What?

Really. Just try to dive into it and it will teach you many things about life.

> Your code sucks!

It's deliberately written this way, so that electrical engineers and embedded system developers can understand it.

> But I am one, and it still looks like Greek to me!

Thats because quite of it is Greek, indeed.

> It has bugs!

Of course. That's the essence of Open Source Software.

> The essence of Open Source Software is having bugs?

Yes. I write bugs, you submit PRs with bug fixes, I get the big VC money, you get all the joy of knowledge, collaboration, the feeling of belnonginess and contribution to a community. That's the beauty of Open Source.

> You have anything to say to PM Mitsotakis?

He knows.

### The Hardware

You will need two Raspberry PICOs, some jumper cables and one USB OTG adapter to connect the keyboard to the host-side PICO 

> How do I connect them? 

Figure it out from the code, it's trivial.

### The Mitsoware

Everything is included in this repo. 
