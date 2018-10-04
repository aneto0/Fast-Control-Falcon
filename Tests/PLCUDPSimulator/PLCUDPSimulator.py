import binascii
import PySimpleGUI27 as sg
import select
import signal
import socket
import struct
import sys
import threading
import time 

class UDPSubscriber (threading.Thread):

    def __init__(self, window):
        threading.Thread.__init__(self)
        self.window = window
        sourcePort = int(window.FindElement('sourcePort').Get())
        self.server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server.setblocking(0)
        self.server.bind(('', sourcePort))
        self.terminateServer = False

    def run(self):
        timeout = 1
        while(not self.terminateServer):
            ready = select.select([self.server], [], [], timeout)
            if ready[0]:
                #datagram = self.rfile.readline().strip()
                data, addr = self.server.recvfrom(4096) # buffer size is 1024 bytes
                headerUID = binascii.hexlify(data[0:7])
                self.window.FindElement('subSDNHUID').Update(headerUID) 
                header = struct.unpack('<4I3Q', data[8:48])
                self.window.FindElement('subSDNHCNT').Update('{0:0x}'.format(header[0]))
                self.window.FindElement('subSDNHTPU').Update('{0:0x}'.format(header[1]))
                self.window.FindElement('subSDNHTPV').Update('{0:0x}'.format(header[2]))
                self.window.FindElement('subSDNHTPS').Update('{0:0x}'.format(header[3]))
                self.window.FindElement('subSDNHCNT').Update('{0:0x}'.format(header[4]))
                self.window.FindElement('subSDNHSDT').Update('{0:0x}'.format(header[5]))
                self.window.FindElement('subSDNHRCT').Update('{0:0x}'.format(header[6]))
                plcHeader = struct.unpack('<8B', data[48:56])
                self.window.FindElement('subHNF').Update('{0:0x}'.format(plcHeader[0]))
                self.window.FindElement('subHNI').Update('{0:0x}'.format(plcHeader[1]))
                self.window.FindElement('subHNC').Update('{0:0x}'.format(plcHeader[2]))
                self.window.FindElement('subHNS').Update('{0:0x}'.format(plcHeader[3]))
                self.window.FindElement('subHNA').Update('{0:0x}'.format(plcHeader[4]))
                self.window.FindElement('subHSP').Update('{0:0x}{1:0x}{2:0x}'.format(plcHeader[5], plcHeader[6], plcHeader[7]))
                payload = struct.unpack('<fH9B', data[56:74])
                self.window.FindElement('subFLT0').Update('{0}'.format(payload[0]))
                self.window.FindElement('subINT0').Update('{0:0x}'.format(payload[1]))
                self.window.FindElement('subACKA').Update('{0:0x}'.format(payload[2]))
                self.window.FindElement('subACKL').Update('{0:0x}'.format(payload[3]))
                self.window.FindElement('subACKR').Update('{0:0x}'.format(payload[4]))
                self.window.FindElement('subACKSP').Update('{0:0x}'.format(payload[5]))
                self.window.FindElement('subACKEP').Update('{0:0x}'.format(payload[6]))
                self.window.FindElement('subACKSU').Update('{0:0x}'.format(payload[7]))
                self.window.FindElement('subSTMT').Update('{0:0x}'.format(payload[8]))
                self.window.FindElement('subSTRT').Update('{0:0x}'.format(payload[9]))
                self.window.FindElement('subALA0').Update('{0:0x}'.format(payload[10]))
                print("Datagram Recieved from client is:".format(data))
        self.server.close()
        self.terminateServer = False

udpSubscriber = None
def terminateProgram():
    print 'Terminating program'
    if (udpSubscriber is not None):
        udpSubscriber.terminateServer = True
        while (udpSubscriber.terminateServer):
            time.sleep(0.5)
    sys.exit(0)

subscriberLayout = [
                    [sg.Text('Source Port'), sg.InputText('2002', key = 'sourcePort', do_not_clear = True)],
                    [   
                        sg.Text('HUID'), sg.InputText('SDNv2.x', key = 'subSDNHUID', do_not_clear = True, size = (16, None)), 
                        sg.Text('HSZE'), sg.InputText('30', key = 'subSDNHUID', do_not_clear = True, size = (4, None)),
                        sg.Text('HTPU'), sg.InputText(key = 'subSDNHTPU', do_not_clear = True, size = (8, None)),
                        sg.Text('HTPV'), sg.InputText(key = 'subSDNHTPV', do_not_clear = True, size = (8, None)),
                        sg.Text('HTPS'), sg.InputText(key = 'subSDNHTPS', do_not_clear = True, size = (8, None))
                    ],
                    [
                        sg.Text('HCNT'), sg.InputText(key = 'subSDNHCNT', do_not_clear = True, size = (20, None)),
                        sg.Text('HSDT'), sg.InputText(key = 'subSDNHSDT', do_not_clear = True, size = (20, None)),
                        sg.Text('HRCT'), sg.InputText(key = 'subSDNHRCT', do_not_clear = True, size = (20, None))
                    ],
                    [   
                        sg.Text('HNF'), sg.InputText(key = 'subHNF', do_not_clear = True, size = (4, None)), 
                        sg.Text('HNI'), sg.InputText(key = 'subHNI', do_not_clear = True, size = (4, None)),
                        sg.Text('HNC'), sg.InputText(key = 'subHNC', do_not_clear = True, size = (4, None)),
                        sg.Text('HNS'), sg.InputText(key = 'subHNS', do_not_clear = True, size = (4, None)),
                        sg.Text('HNA'), sg.InputText(key = 'subHNA', do_not_clear = True, size = (4, None)),
                        sg.Text('HSP'), sg.InputText(key = 'subHSP', do_not_clear = True, size = (4, None))
                    ],
                    [   
                        sg.Text('FLT0'), sg.InputText(key = 'subFLT0', do_not_clear = True, size = (4, None)), 
                        sg.Text('INT0'), sg.InputText(key = 'subINT0', do_not_clear = True, size = (4, None)),
                        sg.Text('ACKA'), sg.InputText(key = 'subACKA', do_not_clear = True, size = (4, None)),
                        sg.Text('ACKL'), sg.InputText(key = 'subACKL', do_not_clear = True, size = (4, None)),
                        sg.Text('ACKR'), sg.InputText(key = 'subACKR', do_not_clear = True, size = (4, None)),
                        sg.Text('ACKSP'), sg.InputText(key = 'subACKSP', do_not_clear = True, size = (4, None)),
                        sg.Text('ACKEP'), sg.InputText(key = 'subACKEP', do_not_clear = True, size = (4, None)),
                        sg.Text('ACKSU'), sg.InputText(key = 'subACKSU', do_not_clear = True, size = (4, None)),
                        sg.Text('STMT'), sg.InputText(key = 'subSTMT', do_not_clear = True, size = (4, None)),
                        sg.Text('STRT'), sg.InputText(key = 'subSTRT', do_not_clear = True, size = (4, None)),
                        sg.Text('ALA0'), sg.InputText(key = 'subALA0', do_not_clear = True, size = (4, None))
                    ],

                    [sg.RButton('Subscribe', key = 'subscribe')]
                 ]

publisherLayout = [
                    [sg.Text('Destination IP'), sg.InputText('127.0.0.1', key = 'destinationIP', do_not_clear = True), sg.Text('Destination Port'), sg.InputText('2000', key = 'destinationPort', do_not_clear = True)],
                    [   
                        sg.Text('HUID'), sg.InputText(binascii.hexlify('SDNv2.x'), key = 'pubSDNHUID', do_not_clear = True, size = (16, None)), 
                        sg.Text('HSZE'), sg.InputText('30', key = 'pubSDNHSZE', do_not_clear = True, size = (4, None)),
                        sg.Text('HTPU'), sg.InputText('0', key = 'pubSDNHTPU', do_not_clear = True, size = (8, None)),
                        sg.Text('HTPV'), sg.InputText('0', key = 'pubSDNHTPV', do_not_clear = True, size = (8, None)),
                        sg.Text('HTPS'), sg.InputText('0', key = 'pubSDNHTPS', do_not_clear = True, size = (8, None))
                    ],
                    [ 
                        sg.Text('HCNT'), sg.InputText('0', key = 'pubSDNHCNT', do_not_clear = True, size = (20, None)),
                        sg.Text('HSDT'), sg.InputText('0', key = 'pubSDNHSDT', do_not_clear = True, size = (20, None)),
                        sg.Text('HRCT'), sg.InputText('0', key = 'pubSDNHRCT', do_not_clear = True, size = (20, None))
                    ],
                    [   
                        sg.Text('HNF'), sg.InputText('1', key = 'pubHNF', do_not_clear = True, size = (4, None)), 
                        sg.Text('HNI'), sg.InputText('1', key = 'pubHNI', do_not_clear = True, size = (4, None)),
                        sg.Text('HNC'), sg.InputText('3', key = 'pubHNC', do_not_clear = True, size = (4, None)),
                        sg.Text('HNS'), sg.InputText('1', key = 'pubHNS', do_not_clear = True, size = (4, None)),
                        sg.Text('HNA'), sg.InputText('1', key = 'pubHNA', do_not_clear = True, size = (4, None)),
                        sg.Text('HSP'), sg.InputText('000', key = 'pubHSP', do_not_clear = True, size = (6, None))
                    ],
                    [   
                        sg.Text('FLT0'), sg.InputText('0', key = 'pubFLT0', do_not_clear = True, size = (4, None)), 
                        sg.Text('INT0'), sg.InputText('0', key = 'pubINT0', do_not_clear = True, size = (4, None)),
                        sg.Text('CMDA'), sg.InputText('0', key = 'pubCMDA', do_not_clear = True, size = (4, None)),
                        sg.Text('CMDL'), sg.InputText('0', key = 'pubCMDL', do_not_clear = True, size = (4, None)),
                        sg.Text('CMDR'), sg.InputText('0', key = 'pubCMDR', do_not_clear = True, size = (4, None)),
                        sg.Text('CMDSP'), sg.InputText('0', key = 'pubCMDSP', do_not_clear = True, size = (4, None)),
                        sg.Text('CMDEP'), sg.InputText('0', key = 'pubCMDEP', do_not_clear = True, size = (4, None)),
                        sg.Text('CMDSU'), sg.InputText('0', key = 'pubCMDSU', do_not_clear = True, size = (4, None)),
                        sg.Text('STPL'), sg.InputText('0', key = 'pubSTPL', do_not_clear = True, size = (4, None)),
                        sg.Text('ALA0'), sg.InputText('0', key = 'pubALA0', do_not_clear = True, size = (4, None))
                    ],
                    [sg.RButton('Publish')]
                ]

layout = [[sg.Frame('Subscriber', subscriberLayout, font='Any 12', title_color='blue')],
          [sg.Frame('Publisher', publisherLayout, font='Any 12', title_color='red')],
          [sg.Exit()]]

window = sg.Window('PLCSDN Emulator').Layout(layout)

signal.signal(signal.SIGTERM, terminateProgram)
signal.signal(signal.SIGINT, terminateProgram)

while True:
    button, values = window.Read()
    if button is 'Publish':
        pubCounter = int(values['pubSDNHCNT'], 16)
        pubCounter = pubCounter + 1
        sendTime = long(time.time() * 1e9)
        window.FindElement('pubSDNHCNT').Update('{0:0x}'.format(pubCounter))
        window.FindElement('pubSDNHSDT').Update('{0:0x}'.format(sendTime))

        packetStr = values['pubSDNHUID'] + '00'
        packetStr += '{0:0>8}'.format(values['pubSDNHSZE'])
        packetStr += '{0:0>8}'.format(values['pubSDNHTPU'])
        packetStr += '{0:0>8}'.format(values['pubSDNHTPV'])
        packetStr += '{0:0>8}'.format(values['pubSDNHTPS'])
        packetStr += '{0:016x}'.format(pubCounter)
        packetStr += '{0:016x}'.format(sendTime)
        packetStr += '{0:0>16}'.format(values['pubSDNHRCT'])

        packetStr += '{0:0>2}'.format(values['pubHNF'])
        packetStr += '{0:0>2}'.format(values['pubHNI'])
        packetStr += '{0:0>2}'.format(values['pubHNC'])
        packetStr += '{0:0>2}'.format(values['pubHNS'])
        packetStr += '{0:0>2}'.format(values['pubHNA'])
        packetStr += '{0:0>6}'.format(values['pubHSP'])

        packetStr += '{0:0>8}'.format(values['pubFLT0'])
        packetStr += '{0:0>4}'.format(values['pubINT0'])
        packetStr += '{0:0>2}'.format(values['pubCMDA'])
        packetStr += '{0:0>2}'.format(values['pubCMDL'])
        packetStr += '{0:0>2}'.format(values['pubCMDR'])
        packetStr += '{0:0>2}'.format(values['pubCMDSP'])
        packetStr += '{0:0>2}'.format(values['pubCMDEP'])
        packetStr += '{0:0>2}'.format(values['pubCMDSU'])
        packetStr += '{0:0>2}'.format(values['pubSTPL'])
        packetStr += '{0:0>2}'.format(values['pubALA0'])

        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
        ipAddrDest = window.FindElement('destinationIP').Get()
        ipPortDest = int(window.FindElement('destinationPort').Get())
        print "Connecting to {0}:{1}".format(ipAddrDest, ipPortDest)
        s.connect((ipAddrDest, ipPortDest))
        packetData = binascii.unhexlify(packetStr)
        s.send(packetData)
        #s.close()
        print packetStr
    elif button is 'subscribe':
        if (udpSubscriber is None):
            udpSubscriber = UDPSubscriber(window)
            udpSubscriber.start()
            window.FindElement('subscribe').Update(disabled = True) 
    elif button is None:
        break

terminateProgram()
