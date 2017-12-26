function out = generateNewGraphics()
   fileNames = ["uniform_equal", ...
       "uniform_0,3", ...
       "normal_equal", ...
       "normal_0,5", ...
       "chi_squared_equal", ...
       "chi_squared_1", ...
       "mixed_equal", ...
       "mixed_disequal"];
   
   lines = generateTestValuesMean();
    lines2 = generateTestValuesMw();
    
    lines = cat(1, lines,lines2);
    disp (size(lines));
   
   fileCount = size(fileNames);
   for num = 2:2:fileCount(2)
      generateImage(fileNames, lines, num);
   end
   
end

function out = generateTestValuesMean()

    out = zeros(8,1);
  
    out(1) = 0.95;
    out(3) = 0.95;
    out(5) = 0.95;
    out(7) = 0.95;
    disp(out);
    
    res = 0;
    for a = 1:1000 
        sample1 = rand(50, 1); 
        sample2 = 0.1 + rand(50, 1);
        res = res + (runTestMean(sample1, sample2));
    end
  %  disp (res);
    out(2) = res / 1000;
    
    res = 0;
    for a = 1:1000 
        sample1 = normrnd(0, 1, 50, 1);
        sample2 = normrnd(0.5, 1, 50, 1);
        res = res + (runTestMean(sample1, sample2));
    end
    out(4) = res / 1000;
    
    res = 0;
    for a = 1:1000 
        sample1 = chi2rnd(3, 50, 1);
        sample2 = chi2rnd(4, 50, 1);
        res = res + (runTestMean(sample1, sample2));
    end
    out(6) = res / 1000;
    
    res = 0;
    for a = 1:1000 
        sample1 = chi2rnd(3, 50, 1);
        sample2 = chi2rnd(4, 50, 1);
        res = res + (runTestMean(sample1, sample2));
    end
    out(6) = res / 1000;

    res = 0;
    for a = 1:1000 
        sample1 = randomFromMixture1(50);
        sample2 = randomFromMixture2(50);
        res = res + (runTestMean(sample1, sample2));
    end
    out(8) = res / 1000;
end

function out = generateTestValuesMw()

    out = zeros(8,1);
  
    out(1) = 0.95;
    out(3) = 0.95;
    out(5) = 0.95;
    out(7) = 0.95;
    disp(out);
    
    res = 0;
    for a = 1:1000 
        sample1 = rand(50, 1); 
        sample2 = 0.1 + rand(50, 1);
        res = res + (runTest(sample1, sample2));
    end
  %  disp (res);
    out(2) = res / 1000;
    
    res = 0;
    for a = 1:1000 
        sample1 = normrnd(0, 1, 50, 1);
        sample2 = normrnd(0.5, 1, 50, 1);
        res = res + (runTest(sample1, sample2));
    end
    out(4) = res / 1000;
    
    res = 0;
    for a = 1:1000 
        sample1 = chi2rnd(3, 50, 1);
        sample2 = chi2rnd(4, 50, 1);
        res = res + (runTest(sample1, sample2));
    end
    out(6) = res / 1000;
    
    res = 0;
    for a = 1:1000 
        sample1 = chi2rnd(3, 50, 1);
        sample2 = chi2rnd(4, 50, 1);
        res = res + (runTest(sample1, sample2));
    end
    out(6) = res / 1000;

    res = 0;
    for a = 1:1000 
        sample1 = randomFromMixture1(50);
        sample2 = randomFromMixture2(50);
        res = res + (runTest(sample1, sample2));
    end
    out(8) = res / 1000;
end
function out = randomFromMixture2(quantity)
    out = zeros(quantity:1);
    for a = 1:quantity
       if (0.5 < rand(1))
           out(a) = normrnd(1, 1);
       else
           out(a) = normrnd(2, 2);
       end
    end
end
function out = randomFromMixture1(quantity)
    out = zeros(quantity:1);
    for a = 1:quantity
       if (0.5 < rand(1))
           out(a) = normrnd(0, 1);
       else
           out(a) = normrnd(0.5, 1);
       end
    end
end

function out = runTest(sample1, sample2)
    [p, out] = ranksum(sample1, sample2);
    out = 1 - out;
end

function out = runTestMean(sample1, sample2)
    out = ttest2(sample1, sample2);
    out = 1 - out;
end

function t = generateImage(nomi, l, num)
%    disp(nome);
    
    dir = "mw2";
    
    figure('visible', 'off');
    fileID = fopen(strcat('../c_output/', dir, '/', nomi(num), ".txt"), 'r+');
    a = fscanf(fileID, "%d %d %d\n");
    fclose(fileID);
    targetFile = strcat('figures/', dir, '/', nomi(num), '.png');
    
    fileID2 = fopen(strcat('../c_output/', dir, '/', nomi(num-1), ".txt"), 'r+');
    a2 = fscanf(fileID2, "%d %d %d\n");
    fclose(fileID2);
   
    iterations = a(1:3:size(a));
    permutations = a(2:3:size(a));
    success = a(3:3:size(a));
    
    iterations2 = a2(1:3:size(a2));
    permutations2 = a2(2:3:size(a2));
    success2 = a2(3:3:size(a2));
    
    map = zeros(10000);
    map2 = zeros(10000);
    
    for c = 1:size(iterations)
        map(permutations(c)) = map(permutations(c)) + success(c);
        map2(permutations2(c)) = map2(permutations2(c)) + success2(c);
    end
    
    x = [];
    y = [];
    z = [];
    
    t = 1;
    while t < 10000
        %disp(map(t)/10000);
        x(size(x)+1) = t;
        y(size(y)+1) = map(t)/10000;
        z(size(z)+1) = 1;

        
        x(size(x)+1) = t;
        y(size(y)+1) = map2(t)/10000;
        z(size(z)+1) = 0.5;
        t = t * 2;
        
        
    end
    scatter(x, 1-y, [], z);
    set(gca, 'XScale', 'log');
    xlabel('permutations');
    ylabel('H0 rejected / total cases');
    name2 = strrep(nomi(num), '_', ' ');
    title(name2);
    %refline(0, l);
    
    %saveas(gcf, targetFile);
    
    %y = 1 - y;
    %scatter(x, y);
    %set(gca, 'XScale', 'log');
    %xlabel('permutations');
    %ylabel('H0 rejected / total cases');
    %title(name2);
    
    line = refline(0, 1-l(num));
    set(line, 'Color', [0, 1, 0]);
    
    line = refline(0, 1-l(num+8));
    set(line, 'Color', [1, 0, 0]);

    
    saveas(gcf, targetFile);
    
end