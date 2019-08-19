clear all
%inputs

%First define the number of nodes
n=6;

%Network topology in form of adjacency matrix
G=[0,1,1,0,0,0;1,0,1,1,0,0;1,1,0,0,1,0;0,1,0,0,1,1;0,0,1,1,0,1;0,0,0,1,1,0];

%Traffic Matrices ODU0, ODU1, ODU2, ODU3, ODU4
D(:,:,1)=[0,5,1,3,1,3;5,0,0,1,5,0;1,0,0,1,4,1;3,1,1,0,1,1;1,5,4,1,0,3;3,0,1,1,3,0];
D(:,:,2)=[0,2,4,2,0,5;2,0,0,3,1,1;4,0,0,1,1,0;2,3,1,0,1,3;0,1,1,1,0,1;5,1,0,3,1,0];
D(:,:,3)=[0,1,1,1,0,0;1,0,0,0,1,0;1,0,0,1,1,0;1,0,1,0,1,0;0,1,1,1,0,1;0,0,0,0,1,0];
D(:,:,4)=[0,0,0,0,0,0;0,0,1,0,0,1;0,1,0,0,1,0;0,0,0,0,0,0;0,0,1,0,0,0;0,1,0,0,0,0];
D(:,:,5)=[0,0,0,0,0,0;0,0,0,0,0,1;0,0,0,0,0,0;0,0,0,0,0,0;0,0,0,0,0,1;0,1,0,0,1,0];

%Vector with the number of amplifier per link
%NR = Dist/100 - 1;
NR=[4,6,0,0,0,0,6,0,0,0,8,0,1,7,3];

%Costs
cust_olt = 15000;
cust_transp = 5000;
cust_amplifier = 4000;
cust_EXC_port = 1000;
cust_EXC_port_ODU0 = 8;
cust_EXC_port_ODU1 = 15;
cust_EXC_port_ODU2 = 30;
cust_EXC_port_ODU3 = 60;
cust_EXC_port_ODU4 = 100;
cust_EXC = 10000;
cust_OXC_port = 2500;
cust_OXC = 20000;

%Variables
number_flows = (n * (n-1))/2;               % number of possible bidirectional demands, i.e. number of possible od pairs

var_f = number_flows * (n * (n-1));         % number f^{od}_{ij} variables (unideritonal)
var_W = number_flows;                       % number W_{ij} variables (bidirectional)
var_L = number_flows;                       % number L_{ij} variables

%Total number of variables
total_var = var_f + var_W + var_L;

% this function is going to create a new ILP with total_var variables
ilp=mxlpsolve('make_lp', 0, total_var);

%OBJECTIVE FUNCTION
f_row = ones(1,total_var);

f_row(1,1:var_f) = 2*cust_OXC_port;
f_row(1,var_f+1:var_W) = cust_OXC + cust_OXC_port + 2*cust_transp*100;
f_row(1,var_f+var_W+1:total_var) = NR*cust_amplifier + 2*cust_olt;

mxlpsolve('set_obj_fn', ilp, f_row);

%CONSTRAINTS
%GROOMING CONSTRAINTS
for o=1:n
    for d=o+1:n
        index_sum = [];      
        f_row = zeros(1,total_var);
        bandwith = 1.25 * D(o,d,1) + 2.5 * D(o,d,2) + 10 * D(o,d,3) + 40 * D(o,d,4) + 100 * D(o,d,5);
        %variable W(o,d)
        W_index = index_calculation(o,d,n);
        W_index = var_f + W_index;
        f_row(W_index)=100;
        mxlpsolve('add_constraint', ilp, f_row, 2, bandwith);
    end
end
        
%FLOW CONSERVATION CONSTRAINTS
for o=1:n   
    for d=o+1:n
        for i=1:n 
            %ORIGIN NODES
            if(i==o)
                %sum over all i
                index_sum = [];
                f_row = zeros(1,total_var);
                for j=1:n
                  if(j~=o && G(i,j)==1)
                    f_index = index_calculation2(i,j,o,d,n);
                    index_sum = [index_sum, f_index];
                  end
                end
                f_row(index_sum)=1;
                %variable W(o,d)
                W_index = index_calculation(o,d,n);
                W_index = var_f + W_index;
                f_row(W_index)=-1;
                mxlpsolve('add_constraint', ilp, f_row, 3, 0);
            
             %INTERMEDIATE NODES
             elseif (i ~=o && i ~=d)
                index_sum = [];
                f_row = zeros(1,total_var);
                %income flow
                for j=1:n
                  if(j~=o && G(i,j)==1) 
                    f_index = index_calculation2(i,j,o,d,n);
                    index_sum = [index_sum, f_index];
                  end
                end
                f_row(index_sum)=1;
                
                %outcome flow   
                index_sum = [];
                for j=1:n
                  if(j~=d && G(i,j)==1)
                    f_index = index_calculation2(j,i,o,d,n);
                    index_sum = [index_sum, f_index];
                  end
                end
                f_row(index_sum)=-1;
                mxlpsolve('add_constraint', ilp, f_row, 3, 0);
                 
            %DESTINATION NODES    
            elseif (i==d)
                index_sum = [];
                f_row = zeros(1,total_var);
                for j=1:n
                  if(j~=d && G(i,j)==1)
                    f_index = index_calculation2(j,i,o,d,n);
                    index_sum = [index_sum, f_index];
                  end
                end
                f_row(index_sum)=1;
                %variable W(o,d)
                W_index = index_calculation(o,d,n);
                W_index = var_f + W_index;
                f_row(W_index)=-1;
                mxlpsolve('add_constraint', ilp, f_row, 3, 0);     
            end
        end
    end
end

%CAPACITY CONSTRAINTS
for i=1:n
    for j=i+1:n    
        index_sum = [];
        f_row = zeros(1,total_var);
        for o=1:n
            for d=o+1:n
                f_index1 = index_calculation2(i,j,o,d,n);
                f_index2 = index_calculation2(j,i,o,d,n);
                index_sum = [index_sum, f_index1, f_index2];
             end
        end
        f_row(index_sum)=1;
        
        L_index = index_calculation(i,j,n);
        L_index = var_f + var_W + L_index;
        f_row(L_index)=1;
        mxlpsolve('add_constraint', ilp, f_row, 1, 100*G(i,j)*f_row(L_index));
    end
end

%BINARY VARIABLES DEFINITION
for i=1:var_L
    mxlpsolve('set_binary', ilp, var_f+var_W+i, 1);
end

%INTEGER VARIABLES DEFINITION
for i=1:var_f
    mxlpsolve('set_int', ilp, i, 1);
end

for i=1:var_W
    mxlpsolve('set_int', ilp, var_f+i, 1);
end

mxlpsolve('write_lp', ilp, 'transparent_without_survivability_ref_LT.lp');
mxlpsolve('solve', ilp);
obj = mxlpsolve('get_objective', ilp);
var = mxlpsolve('get_variables', ilp);

% SHOW RESULTS
fprintf('\n\nRESULTS: Reference Network\n');
fprintf('\nScenario: Transparent without survivability - Low Traffic\n\n');

fprintf('--------------------------------------------------------\n');
fprintf('Information regarding LINKS: \n');
fprintf('--------------------------------------------------------\n');
%variable f_ij^od - number of optical channels per link
W_matrix = zeros(n,n);
for i=1:n
    for j=i+1:n
        if G(i,j)==1
            for o=1:n
                for d=o+1:n
                    W_matrix(i,j) = W_matrix(i,j) + var(index_calculation2(i,j,o,d,n)) + var(index_calculation2(j,i,o,d,n));
                    W_matrix(j,i) = W_matrix(i,j);
                end        
            end 
        end
    end
end
%variable W_od - number of optical channels per demand
Wod_matrix = zeros(n,n);
for o=1:n
    for d=o+1:n
        Wod_matrix(o,d) = var(var_f+index_calculation(o,d,n));
        Wod_matrix(d,o) = Wod_matrix(o,d);
    end
end
total_W = 0;
Z=0;
fprintf('| Link between Node: | Optical channels: | Amplifiers: |\n')
for i=1:n
    for j=i+1:n
        Z=Z+1;
        if G(i,j)==1
            fprintf('| Node %d <-> Node %d  | \t\t %d \t\t\t | \t\t %d     |\n', i, j, W_matrix(i,j), NR(Z));
            total_W = total_W + W_matrix(i,j);
        end
    end
end
fprintf('\n');
fprintf('-------------------------------------------------------\n');
for o=1:n
    for d=o+1:n
        fprintf('Number of lightpaths between node (%d,%d): %d\n', o, d, Wod_matrix(o,d));
    end
end
fprintf('-------------------------------------------------------\n');

fprintf('------------------------------------------------------\n');
fprintf('Information regarding NODES:\n');
fprintf('------------------------------------------------------\n');

%Information about connections between nodes
connect=zeros(n,1);
for i=1:n
    for j=1:n
        if G(i,j)==1 & W_matrix(i,j)>0
            connect(i)=connect(i)+1;
        end
    end
end

%information about tributary ports
sum_trib=zeros(n,1);
for i=1:n
    aux_trib_ports(i,1) = i;
    for j=1:5
        aux_trib_ports(i,j+1) = sum(D(i,:,j));
        sum_trib(i) = sum_trib(i) + aux_trib_ports(i,j+1);
    end
end

%information about line ports
LINE = 0;
for i=1:n
    aux_line_ports(i,1) = i;
    aux_line_ports(i,2) = sum(W_matrix(i,:));
    LINE = LINE + aux_line_ports(i,2);
end

%information about add ports
ADD = 0;
for i=1:n
    aux_add_ports(i,1) = i;
    aux_add_ports(i,2) = sum(Wod_matrix(i,:));
    ADD = ADD + aux_add_ports(i,2);
end

%print information
fprintf('Node: | Connections: | Line Ports: | Add Ports: | Tributary Ports: |\n');
for i=1:n
    fprintf('  %d   | \t %d \t\t| \t  %d \t  |\t  %d \t  | \t %d \t\t |\n',i,connect(i),aux_line_ports(i,2),aux_add_ports(i,2),sum_trib(i));    
end
fprintf('\n');

fprintf('Detailed description of each node:\n\n');
for i=1:n
    fprintf('Node %d:\n',i);
    fprintf('\t- Needs %d line ports.\n',aux_line_ports(i,2));
    for j=1:n
        if G(i,j)==1 & W_matrix(i,j)>0
            fprintf('\t\t- %d connect to Node %d with 100 Gbits/s \n',W_matrix(i,j),j);
        end
    end
    fprintf('\t- Needs %d add ports.\n',aux_add_ports(i,2));
    for j=1:n
        if Wod_matrix(i,j)>0
        fprintf('\t\t- %d connect to Node %d \n',Wod_matrix(i,j),j);
        end
    end
    fprintf('\t- Needs %d tributary ports:\n',sum_trib(i));
    for j=1:5
        if aux_trib_ports(i,j+1)>0
            fprintf('\t\t -Where %d is the ODU%d;\n',aux_trib_ports(i,j+1),j-1);
        end
    end
    fprintf('\n');
end
fprintf('\n');

fprintf('-------------------------------------------------------------\n');
fprintf('Information regarding PATHS:\n');
fprintf('-------------------------------------------------------------\n');
for o=1:n
    for d=o+1:n
        fprintf('Path between Node%d <-> Node%d:\n',o,d);
        for i=1:n
            for j=i+1:n
                if G(i,j)==1
                    if var(index_calculation2(i,j,o,d,n)) ~= 0
                        fprintf('-Link(%d,%d) ',i,j);
                    elseif var(index_calculation2(j,i,o,d,n)) ~= 0
                        fprintf('-Link(%d,%d) ',j,i);
                    end
                end
            end
        end
        fprintf('\n\n');
    end
end
fprintf('\n');

fprintf('-------------------------------------------------------------\n');
fprintf('Information regarding CAPEX:\n');
fprintf('-------------------------------------------------------------\n');
%Calculation of bidirectional network traffic - T1
Traffic = 0; P_ODU0 = 0; P_ODU1 = 0; P_ODU2 = 0; P_ODU3 = 0; P_ODU4 = 0;
for o=1:n
    for d=1:n
        aux = 1.25 * D(o,d,1) + 2.5 * D(o,d,2) + 10 * D(o,d,3) + 40 * D(o,d,4) + 100 * D(o,d,5);
        Traffic = Traffic + aux;
        P_ODU0 = P_ODU0 + D(o,d,1);
        P_ODU1 = P_ODU1 + D(o,d,2);
        P_ODU2 = P_ODU2 + D(o,d,3);
        P_ODU3 = P_ODU3 + D(o,d,4);
        P_ODU4 = P_ODU4 + D(o,d,5);        
    end
end

%Number of link used and Number of total amplifiers
real_NR = zeros(number_flows,1);
total_L = 0;
Y = 0;
for i=1:n
    for j=i+1:n
        Y = Y+1;
        if G(i,j)==1 & W_matrix(i,j)>0
            total_L = total_L + 1;
            real_NR(Y) = NR(Y);
        end
        if G(i,j)==1 & W_matrix(i,j)==0
            real_NR(Y) = 0;
        end
    end
end
num_amplifier = sum(real_NR);

%Calculation of CAPEX
%Cost OLT
olt = 2 * cust_olt * total_L;
%Cost Transponder
transponder = 2 * cust_transp * 100 * total_W;
%Cost Amplifier
amplifiers = 2 * num_amplifier * cust_amplifier;
%Link Cost
linkcost = olt + transponder + amplifiers;

%electrical cost
C_Trib0 = P_ODU0 * 1.25 * cust_EXC_port_ODU0;
C_Trib1 = P_ODU1 * 2.5 * cust_EXC_port_ODU1;
C_Trib2 = P_ODU2 * 10 * cust_EXC_port_ODU2;
C_Trib3 = P_ODU3 * 40 * cust_EXC_port_ODU3;
C_Trib4 = P_ODU4 * 100 * cust_EXC_port_ODU4;
C_LINE = LINE * 100 * cust_EXC_port;

electrical = (cust_EXC * n) + (C_Trib0 + C_Trib1 + C_Trib2 + C_Trib3 + C_Trib4 + C_LINE);

%optical cost
optical =(cust_OXC * n) + cust_OXC_port*(ADD + 2*LINE);

%Node Cost
nodecost = electrical + optical;

%CAPEX
capex = linkcost + nodecost;

%NETWORK COST
fprintf('-------------------------------------------------------------\n');
fprintf('-------------------- Link Cost: -----------------------------\n');
fprintf('-------------------------------------------------------------\n');
fprintf('\t|\t\t\t   | number  | unit price   | cust      |\n');
fprintf('\t| OLT:         |  %5d  |    %5d �   | %5d �  |\n',2*total_L,cust_olt,olt);
fprintf('\t| Transponders:|  %5d  |    %5d �   | %5d � |\n',2*total_W,cust_transp,transponder);
fprintf('\t| Amplifiers:  |  %5d  |    %5d �   | %5d �  |\n',2*num_amplifier,cust_amplifier,amplifiers);
fprintf('-------------------------------------------------------------\n');
fprintf('\t-- Total Link Cost:  %5d �             \n',linkcost);
fprintf('-------------------------------------------------------------\n');
fprintf('-------------------------------------------------------------\n');
fprintf('---------------------- Node Cost: ---------------------------\n');
fprintf('-------------------------------------------------------------\n');
fprintf('\t|\t\t\t\t    | number  | unit price   | cust      |\n');
fprintf('\t| EXCs:|  %5d  |    %5d �   | %5d � |\n',n,cust_EXC,n*cust_EXC);
fprintf('\t| ODU0 Ports:|  %5d  |    %5d �   | %5d �   |\n',P_ODU0,cust_EXC_port_ODU0,C_Trib0);
fprintf('\t| ODU1 Ports:|  %5d  |    %5d �   | %5d �   |\n',P_ODU1,cust_EXC_port_ODU1,C_Trib1);
fprintf('\t| ODU2 Ports:|  %5d  |    %5d �   | %5d �   |\n',P_ODU2,cust_EXC_port_ODU2,C_Trib2);
fprintf('\t| ODU3 Ports:|  %5d  |    %5.1d �   | %5d �   |\n',P_ODU3,cust_EXC_port_ODU3,C_Trib3);
fprintf('\t| ODU4 Ports:|  %5d  |    %5d �   | %5d �   |\n',P_ODU4,cust_EXC_port_ODU4,C_Trib4);
fprintf('\t| Line Ports:|  %5d  |    %5d �   | %5d �   |\n',LINE,cust_EXC_port,C_LINE);
fprintf('-------------------------------------------------------------\n');
fprintf('\t--  Total Electrical:         %5d �             \n',electrical);
fprintf('-------------------------------------------------------------\n');
fprintf('\t| OXCs:|  %5d  |    %5d �   | %5d � |\n',n,cust_OXC,n*cust_OXC);
fprintf('\t| Line Ports:|  %5d  |    %5d �   | %5d � |\n',2*LINE,cust_OXC_port,2*LINE*cust_OXC_port);
fprintf('\t| Add Ports:|  %5d  |    %5d �   | %5d � |\n',ADD,cust_OXC_port,ADD*cust_OXC_port);
fprintf('-------------------------------------------------------------\n');
fprintf('\t--  Total Optical:         %5d �             \n',optical);
fprintf('-------------------------------------------------------------\n');
fprintf('\t-- Total Node Cost:  %5d �             \n',nodecost);
fprintf('-------------------------------------------------------------\n');
fprintf('\t------- Total Network Cost: %5d �          \n',capex);
fprintf('-------------------------------------------------------------\n');
%Tributary_Ports = array2table(aux_trib_ports,'VariableNames',{'Node' 'ODU0' 'ODU1' 'ODU2' 'ODU3' 'ODU4'})
%Line_Ports = array2table(aux_thr_ports,'VariableNames',{'Node' 'OTU4'})
%fprintf('Traffic: %d\n',Traffic);
