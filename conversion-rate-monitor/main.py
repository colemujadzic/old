from modules import xmltodict
from sys import exit
import urllib2
import time
import numbers


#scheduler
startTime = time.time()

#site to parse XML from
site = "http://rates.fxcm.com/RatesXML"

#header used to spoof as a browser
header = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11',
          'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
          'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
          'Accept-Encoding': 'none',
          'Accept-Language': 'en-US,en;q=0.8',
          'Connection': 'keep-alive'}

class RateMonitor(object):
    # used to check that the user entered a valid pair
    def checkPair(self, pair):
        request = urllib2.Request(site, headers=header)

        try:
            file = urllib2.urlopen(request)
        except urllib2.HTTPError, e:
            print e.fp.read()

        data = file.read()
        file.close()

        rateData = xmltodict.parse(data)

        for Rate in rateData['Rates']['Rate']:
            if Rate['@Symbol'] == pair:
                return True
        print ("The pair (" + pair + ") you entered is invalid please try again.")
        return False

    # Tests whether the number entered is a real number
    def inputTargetRate(self, prompt):
        passed = False
        num = 0
        while (not passed):
            try:
                num = float(raw_input(prompt))
                passed = True
            except ValueError:
                print("That is not a valid number.  Please try again.")
        return str(num)

    #function used to parse the XML file, print rate information
    def checkRate(self, pair):
        request = urllib2.Request(site, headers=header)

        try:
            file = urllib2.urlopen(request)
        except urllib2.HTTPError, e:
            print e.fp.read()

        data = file.read()
        file.close()

        rateData = xmltodict.parse(data)

        for Rate in rateData['Rates']['Rate']:
            if Rate['@Symbol'] == pair:
                currentRate = Rate['Bid']
                print '------------------------------------------------------------'
                print("Current rate of " + pair + " is " + currentRate)
                suggestedSellRate = Rate['Ask']
                print("Suggested sell rate of " + pair + " is " + suggestedSellRate)
                dailyHigh = Rate['High']
                print("Daily high of " + pair + " is " + dailyHigh)
                dailyLow = Rate['Low']
                print("Daily low of " + pair + " is " + dailyLow)

                direction = Rate['Direction']
                if direction == '1':
                    print("Rate of " + pair + " has increased since last rate ")
                elif direction == '-1':
                    print("Rate of " + pair + " has decreased since last rate ")
                elif direction == '0':
                    print("Rate of " + pair + " has not changed since last rate ")

                lastTick = Rate['Last']
                print("Time for last tick of " + pair + " is " + lastTick)
                print '------------------------------------------------------------'

    #function used to parse the XML file, check for target rate
    def parseXML(self, pair, targetRate):
        request = urllib2.Request(site, headers=header)

        try:
            file = urllib2.urlopen(request)
        except urllib2.HTTPError, e:
            print e.fp.read()

        data = file.read()
        file.close()

        rateData = xmltodict.parse(data)

        currentRate = None

        #Parses the python dictionary for the pair's current rate
        #, prints and exits when target rate is reached
        for Rate in rateData['Rates']['Rate']:
            if Rate['@Symbol'] == pair:
                if Rate['Bid'] >= targetRate:
                    currentRate = Rate['Bid']
                    print 'Target rate reached!'
                    print("The current rate of " + pair + " is " + currentRate)
                    print '------------------------------------------------------------'
                    exit()

if __name__ == '__main__':
    check = RateMonitor()
    checkIfValid = False
    while (not checkIfValid):
        pair = raw_input('Enter a pair: ')
        checkIfValid = check.checkPair(pair)

    check.checkRate(pair)
    targetRate = check.inputTargetRate('Enter a target rate: ')
    print '------------------------------------------------------------'


    #checks for changes in current rate every 10 seconds
    while True:
        check.parseXML(pair, targetRate)
        time.sleep(10.0 - ((time.time() - startTime) % 10.0))
