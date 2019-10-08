//######################### NUMBER OF NODES ####################################
numberOfNodes=6
//################# PHYSICAL TOPOLOGY ADJACENCY MATRIX #########################
physicalTopologyAdjacencyMatrix=6
0 1 0 0 0 1
1 0 1 0 0 1
0 1 0 1 1 0
0 0 1 0 1 0
0 0 1 1 0 1
1 1 0 0 1 0
//###################### DISTANCES BETWEEN NODES (km) ##########################
distancesBetweenNodes=6
0 350 0 0 0 150
350 0 400 0 0 120
0 400 0 250 100 0
0 0 250 0 200 0
0 0 100 200 0 600
150 120 0 0 600 0
//############################ SPAN (km) #######################################
span=100
//########################## NUMBER OF TS PER LINK #############################
numberOfTSPerLink=1
//################### NUMBER OF OPTICAL CHANNELS PER LINK ######################
numberOfOpticalChannelsPerLink=100
//######################### INITIAL WAVELENGTH (nm) ############################
initialWavelenght=1550
//#################### WAVELENGHT SPACING IN NANOMETERS ########################
wavelenghtSpacing=0.8
//################### OPTICAL CHANNEL CAPACITY IN ODU0 #########################
opticalChannelCapacity=80
//##############################################################################
//######################### TRAFFIC MATRIX #####################################
//##############################################################################
//############################# ODU0 ###########################################
odu0=6
0 10 2 6 2 6
10 0 0 2 10 0
2 0 0 2 8 2
6 2 2 0 2 2
2 10 8 2 0 6
6 0 2 2 6 0
//##############################################################################
//############################ OTHER ODU MATRICES ##############################
//##############################################################################
//########################### ORDERING RULE ####################################
orderingRule=descendingOrder
//########################### TRANSPORT MODE ###################################
transportMode=transparent
//################### ROUTING CRITERION LOGICAL TOPOLOGY #######################
routingCriterionLogicalTopology=hops
//################## BLOCKING CRITERION LOGICAL TOPOLOGY #######################
blockingCriterionLogicalTopology=1
//################## ROUTING CRITERION PHYSICAL TOPOLOGY #######################
routingCriterionPhysicalTopology=hops
//################## BLOCKING CRITERION PHYSICAL TOPOLOGY ######################
blockingCriterionPhysicalTopology=3