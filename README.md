# Free-the-World-Algorithm
an algorithm to conduct anonymous elections with billions of authenticated voters securely and verifiable


This page presents an algorithm for realizing votes.
We call it the Free-the-World-Algorithm because we believe it
will change the world eternaly for the better, towards a true democracy.
It gives knowledge and reflection in an instant at a global scale,
with full transparency, security, anonymity, equality, accountablity,
verifiablity, trustablity,...

Anyone will be able to host a trustworthy anonymous vote at any time 
for voting pools of virtually any size.
Examples could be:
- Do you think the earth is flat or round? [My Family and Friends]
- Is XYZ a good person? [Ask all vote-eligible citizens of the USA]
- Shall we save the climate? [every passport-holder on planet earth]
- ...

For our description, we assume a state with a government and citizens,
some of which are eligible to vote. The government creates a vote and
the eligible citizens will each anonymously submit either none or one vote.

We call the government and the voters nodes.

We think of the eligible citizenship as a "voting pool".
That is a public list of public_eligibility_key-s.
Each eligible voter has exactly one public_eligibility_key.
To submit a public_eligibility_key for a voting pool,
you go to the voting pool's manager. He* will look at you,
may ask for your ID, and then receive your public_eligibility_key.
which they immediately publish online with their signature
for everyone and you to see.
(In the real world, the manager is an office where they check
your passport. Reasonably, you must have a passport to vote,
so you must trust into the voting pool manager.
That seems unavoidable because someone must arbitrarily decide
who is eligible for his voting pool 
--which is exactly the voting pool manager's role.
..but you do not have to trust anyone else.
And you can choose your voting pool.)

*her/its/theirs (we use he to avoid confusion with singular/plural)

Any node can take on the role of a government, select a voting pool,
and hold a voting question to that pool. Each individual in the pool
may then participate in the vote.
Everyone on earth can see and verify the voting result online forever.
The anonymity of the vote remains uncompromised forever because
it does not rely on cryptography but sheer lack of information.

The vote is held online on a message board.
For the message board we assume:
- that anyone can post messages there without leaving a trace (e.g., via onion routing)
- that the board cannot be spammed with messages (e.g., via some mild proof of work)
- posted messages stay forever and cannot be altered (e.g., via blockchain with signatures)
Forther, the following techniques will be used:
- crypto-secure hashes
- asymmetric cryptography (public and private key pairs)
- computational challenges
- deadlines (temporal)
- random numbers
- multi-factor authentication for any digital signatures in order to protect against compromised devices

We believe our algorithm is secure, even under any combion of any of 
any amount of currupt conspiring groups of nodes of any size.

In case of curruption or conspiracy or both, the algorithm will
identify with parametric high probability at least one corrupt node.


# How the Algorithm works

The government node (called G henceforth) posts the question on the board.
{"Whom do you want for president: X or Y? [voting_pool:=All_eligible_USA_citizens]"}.sign(G)

G then generates a sorted list LR of pair{random_number,challenge_solved_token}, called "real list". L has length m.
For each element E in voting_pool the government will then do the following:

1) create a shuffled copy LRE of LR and the index list IE to order LR into LRE.

2) create a shuffled list of random numbers FE of length m, called "fake list".

3) create two lists KLRE and KLFE of challenge_key. KLRE and KLFE have length m, respectively.

4) copy LRE into list eLRE, in which each element of LRE is encrypted with the respective challenge_key in KLRE

5) copy LFE into list eLFE, in which each element of LFE is encrypted with the respective challenge_key in KLFE

6) post {{shuffle(eLRE,eLFE)}.encrypted(E.public_eligibility_key),E.public_eligibility_key}.signature(G) on the message board.

Afterwards, G writes a private file in which it lists:
file_of_G:={LR,for each E in voting_pool{IE,LFE,KLRE,KLFE}}

Then, G writes {SHA(file_of_G)}.signature(G) onto the message board.

Each E in voting_pool scans the message board for the message
MGE:={{eLE1,eLE2}.encrypted(E.public_eligibility_key),E.public_eligibility_key}.signature(G)

Notice that E cannot know which list of eLE1 and eLE2 is LRE because the pair has been shuffled.

G posts the message {"Legitimation Phase.",Deadline1,post_index:=0}.signed(G).

Now each node E will do the following:
1) decrypt {eE1,eE2}.encrypted(E.public_eligibility_key) via E.private_eligibility_key

2) create asymmetric key pair public_E1_anonymous_vote_key,private_E1_anonymous_vote_key.

3) create asymmetric key pair public_E2_anonymous_vote_key,private_E2_anonymous_vote_key.

4) create asymmetric key pair public_E1_secondary_key,private_E1_secondary_key.

5) create asymmetric key pair public_E2_secondary_key,private_E2_secondary_key.

6) select two random indices ieE1 and ieE2.

7) solve the computational challenge eLE1[ieE1] by cracking the challenge_key, in order to obtain pair{LE1[ieE1],challenge_solved_token}.

8) solve the computational challenge eLE2[ieE2] by cracking the challenge_key, in order to obtain pair{LEe[ieE2],challenge_solved_token}.

9) post each of the following two messages at a random time (thus implying random order) before the Deadline1 passes:

9.1) {{public_E1_anonymous_vote_key,LE1[ieE1],post_index_E1:=post_index++,"legit"}

9.2) {{public_E2_anonymous_vote_key,LE2[ieE2],post_index_E2:=post_index++,"legit"}

Make sure to use TOR browser or anything that protects your anonymity.
Under no circumstances should E sign any message in 9.1 or 9.2 .
In the notation, LE2[ieE2] is the ieE2-th element of the decrypted list LE2.

If E finds that the message board does not show his messages from 9.1 or 9.2 correctly, then this means there is a corrupted device. E terminates the vote via
message {"compromised device"}.sign(E) . Now there has to be a cyber police investigation on who committed a crime or conducted an error.
Since at this point no voter typed in their vote, there is no risk of any breach of anonymity when all processed data of all involved individuals is disclosed.
Even vote_pools can simply be reinitialized via new submission of public_eligibility_key_s.
Legal prosecution should prevent a case in which a message such as {"compromised device"}.sign(E) can be posted legitimately on a regular basis.

If everything worked out, then everyone waits until Deadline1 passed.

Now each node E will do the following:

10) post each of the following two messages at a random time (thus implying random order) before the Deadline1 passes:

10.1) {{private_E1_secondary_key,LE1[ieE1],post_index_E1,"unlock"}

10.2) {{private_E2_secondary_key,LE2[ieE2],post_index_E2,"unlock"}

Make sure to use TOR browser or anything that protects your anonymity.
Under no circumstances should E sign any message in 10.1 or 10.2 .

For every post_index, G will now decrypt the messages with label "legit" via means of the private_secondary_key-s in the messages with label "unlock".
Thus, G can convert each message with label "legit" into a private file of the following format:

S:={public_anonymous_vote_key,Ei,Ei_post_index}

For each S it does the following:
If S.Ei is an element of L then post the message {S}.signature(G)

Finally, G posts the message {file_of_G}.signature(G) .

Here ends the algorithm.

# Result of the Algorithm
At the end, each voter E holds one private_anonymous_vote_key that he can use to sign votes.
Hence, he can publicly message his vote as {"I want X for president."}.signature(private_anonymous_vote_key).
The public in turn has knowledge of all public_anonymous_vote_key-s, so they can verify every single vote.
They can also see whether the same private_anonymous_vote_key has been used twice in the same voting and simply ignore it.
Again, because there is a proof-of-work cost attached to any anonymous messages, the message board will not be spammed beyond the point of recognition.
Also, using blockchain with signatures, the votes written on the message board remain unalterable for eternaty.
Furthermore, even if all cryptographic information breaks at some point in the future, the anonymity of all voters remains uncompromised.
This is because the anonymity stems from steps 9 and 10, where no encryption (other than within TOR browser) is used to conceil anyone's identity.


# Algorithm Parameters
The algorithm has a couple of parameters:

m : number of voters.

n : length of the real lists and fake lists.

s : number of digits of each random_number

k : number of fake lists. In the above description, k=1, but we can easily extend it to k>1.

d : difficulty of the computational challenges to be solved within finite time Deadline1.

c : proof-of-work cost for posting an unsignatured message.


# Analysis of Computational Cost
G must generate m*k*n random numbers and sort a list of roughly size 2*n.
Each E must solve two computational challenges of difficulty d.


# Analysis of Attacks

G could try to forge special numbers.
This would be detected when G finally publishes file_of_G.

A voter or conspring groups of voters could try to crack many computational challenges to sieve out the fake lists.
If they can identify a definitive member of list L then they can post the messages in step 9 and 10 multiple times in order two gain multiple votes.
However, when n is sufficiently large and d is reasonably large then they are out of luck to gain enough information to compare the lists and reliably identify an element of L.

If, on the other hand, any voter submits two elements of either LE1 or LE2 then with a probability of (k-1)/k they submit an element from a fake list.
Doing so would immediately expose them as currupt. That is because file_of_G shows which lists have been sent to which node E. And assuming that someone else just randomly guessed a correct element from one of your fake lists is beyond reasonable doubt when s is sufficiently large.

G could conspire with voters and tell them the challenge keys.
But then they have to be careful. If, for instance, some voters submit too many messages in 9 then the index may rise beyond 2 times n.
Now when G publishes file_of_G and significantly more than n messages from step 9 contain a feasible number from LR then G is found guilty of fraud beyond reasonable doubt and faces legal prosecution.

Thus, G must trust the entire group of conspiring voters because any of them could expose G by virtue of posting many messages in 9.

Also, if G disobeys to publish file_of_G then the vote is cancelled in any case and further G may be found guilty of fraud and face legal prosecution.
For the above matter, admittedly, it could be possible that G conspires with a *small* group of voters.
But these will not swing a voting. Hence, unless there is insufficient vote participation, the integrity of the vote remains intact.


A non-eligible node could try to vote. In this case, they would have to guess a number that is written in LR.
This is because the final messages S of G, in which the eligible public_anonymous_vote_key-s are published,
depend only on LR. However, when n and s are sufficiently large then the non-eligible node is out of luck to guess a number that is written in LR.
We must also consider that each guess means posting an unsignatured message, which is connected to a proof of work.
For a reasonable value of c this is expensive in scale for many messages. Hence, a non-eligible node cannot simply post billions of guesses.


# Original Scholarship 
The author of this documentation is unaware to any prior algorithm that is similar to the above.
In the below, we print the hash SHA256(7654896978413 Jon Doe), where Jon Doe is replaced with the author's name.

abca3c6b9fca064f6d94f045a4cd732137bc0bfda1b57b9183436b7317713012

