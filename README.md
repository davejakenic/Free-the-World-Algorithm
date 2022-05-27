# Free-the-World-Algorithm

An algorithm to conduct anonymous votings/ pollings/ opinion studies with billions of authenticated voters.
In contrast to, e.g., paper voting, it is cryptographically secure and everyone can verify every vote.


Two questions are solved:
1) The oracle problem solution: N distinct human beings hold one authenticated account each. Notice that an authenticated account cannot be anonymous.
2) The authentication anonymisation procedure: N authenticated accounts are laundered into N anonymous accounts in a 1:1 mapping.

The authentication is based on asymmetric encryption. Our proof-of-concept implementation uses RSA and is merely 1000 LOC.
The laundering is not based on cryptography but randomness. This makes anonymity unpenetrable, even if you have an oracle that can randomly guess everyone's keys!

Our hope for this algorithm is that it will foster a global direct democratic community in which each individual human has exactly the same voice and the same power.
This would end corruption, violence, exploitation, and unfairness in general. It would enable the human species to coordinate global solutions to global problems.
It would counter confirmation bias and misinformation because everyone can ask questions and check what the people really think about a subject.

Examples could be:
- Do you think the earth is flat or round? [Let my Family and Friends vote.]
- Is XYZ a good person? [Ask all vote-eligible citizens of the USA.]
- Shall we save the climate? [Ask every passport-holder on planet earth.]
- ...

In the following two subsections, we describe the two afore-mentioned solution components a bit further.

A detailed description of the method is given in the paper "VotingMethod.pdf" in Section 5.
The LateX code of the paper is provided in "VotingMethod.tex" and is free for everyone to use for any purpose.
A presentation video is linked in "Video_link.txt". The slides/diagrams are provided in "Pres_Voting.pdf" and "drawing-free-vote.pdf".
A proof-of-concept implementation is packaged in "Proof of Concept - Voting Algorithm.zip".
The same code unzipped is provided in the file "Voting Algorithm".


# The Oracle Problem Solution and the Vote Pool

A vote pool is an official list of all those accounts who are eligible to vote.
The "oracle problem" arises when we want to ensure that each account belongs to one distinct real human being.
Otherwise, some people could make several account and thus commit double-voting.

A vote pool exists in every voting system. For instance, in Germany it is the passport system:
The state issues you a passport. This is your proof of eligibility. You need to have it with you when you want to enter the voting booth.

On this website, we explain how we can decentraize the passport issuer into a web of trust.
This is clearly superior in terms of security over a single central governmental passport issuer.
In result, there is a digital passport equivalent that can be used to create vote pools.

As with every passport issuing system, our decentralized passport system also requires each individual to go somewhere where they are measured.
In a classical passport, this measurement is the biometric face image. In our system, we consider the example of an iris scan encoding.


# The Actual Voting System

A voting system can be understood as an anonymous structured message board on which every account is anonymous but owned by one single distinct real human.
To get there, all we need is to anonymize the authenticated accounts of the N voters in the pool.

This is done by the "host". A host is any member in the pool who wants to ask a question.

At the heart of the procedure, the host (e.g., she) will show each voter two lists with random numbers.
Each voter (e.g., he) will secretly pick one number from each list.
For example: List A --> picks number 67483758462875937189074189657864377391047587451ß8309
For example: List B --> picks number 1478902437879232714837245637145463768246356174351418
he does not know which is the true/fake list. We then crafts two RSA key pairs.
He will then make two envelopes and put one public RSA key into each envelope.
Finally, he puts one of the two above numbers in each envelope and anonymously sends them back to the host.

The trick is: One of the two lists was identical for each voter (called "real list"), and one of them was distinct for each voter (called "fake list").
In result:
1) the host can test whether an envelope originated from an eligible voter (because only these know the numbers from the real list)
2) the host can test whether each voter really submitted only one number per list (by virtue of the fake list(s))
3) the voter has certainty beyond cryptography that no-one can tell who submitted his number from the real list


# Accessibility of the Method

A voting algorithm in which every human on earth can trust forbids the presence of sophisticated mathematics.
Today, children learn math in school. We envision a society in which everyone would be able to code their own client code such that no-one has to trust in anyone or anything unless they willingly choose to.

The mathematics involved are merely RSA and random numbers.
In order to trust into the system, users must only comprehend two concepts:
1) RSA cryptography; i.e., the formula with p,q,n,d,e, and the totient.
2) Random numbers; i.e., that there is no way to compute the next lottery numbers.


# Remarks

For those who read the paper: We are aware that commutative cryptography, i.e. e1(e2(M)=e2(e1(M)) could be used to reduce the length n of the lists. But then, commutative encryption violates our principle that anonymity is protected by randomness instead of cryptography. Also, it would make the method more sophisticated and thus less accessible.


# Original Scholarship 

The author of this documentation is unaware to any prior algorithm that is similar to the above.
In the below, we print the hash SHA256(7654896978413 Jon Doe), where Jon Doe is replaced with the author's name.

abca3c6b9fca064f6d94f045a4cd732137bc0bfda1b57b9183436b7317713012

