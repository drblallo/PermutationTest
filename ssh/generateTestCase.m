function generateTestCase()
    rng('shuffle')
    sampleSize = 500;
    vectorSize = 1;
    type = "univariato_gaussiano_cambio_media";
    
    samples1 = normrnd(0, 1, sampleSize, vectorSize); 
    samples2 = normrnd(1, 1, sampleSize, vectorSize);
    
    h = ttest2(samples1, samples2);
    [p, h2] = ranksum(samples1, samples2);
    
    s = strcat(type," ", string(1 == h)," ", string(h2), " ", string(vectorSize));
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);

    
    type = "univariato_gaussiano_cambio_varianza";
    
    samples1 = normrnd(0, 1, sampleSize, vectorSize); 
    samples2 = normrnd(0, 2, sampleSize, vectorSize);
    
    h = ttest2(samples1, samples2);
    [p, h2] = ranksum(samples1, samples2);
    
    s = [s, strcat(type," ", string(h == 1), " ", string(h2), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);

    
    type = "univariato_uniforme_cambio_media";
    
    samples1 = -1 + (2 * rand(sampleSize, vectorSize)); 
    samples2 = 2 * rand(sampleSize, vectorSize);
    
    h = ttest2(samples1, samples2);
    [p, h2] = ranksum(samples1, samples2);
        s = [s, strcat(type, " ", string(h == 1)," ",  string(h2), " ", string(vectorSize))];

    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    type = "univariato_uniforme_cambio_range";
    
    samples1 = -1 + (2 * rand(sampleSize, vectorSize)); 
    samples2 = -2 + (4 * rand(sampleSize, vectorSize));
    
    h = ttest2(samples1, samples2);
    [p, h2] = ranksum(samples1, samples2);
        s = [s, strcat(type," " , string(h == 1), " ", string(h2), " ", string(vectorSize))];

    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    
    type = "univariato_mixture_cambio_media";
    
    samples1 = generateMixtureShifMedia(0, sampleSize, vectorSize); 
    samples2 = generateMixtureShifMedia(1, sampleSize, vectorSize); 
    
    h = ttest2(samples1, samples2);
    [p, h2] = ranksum(samples1, samples2);
        s = [s, strcat(type, " ",string(h == 1), " ", string(h2), " ", string(vectorSize))];

    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    
     type = "univariato_mixture_cambio_allontanamento";
    
    samples1 = generateMixtureAllontanamento(1, sampleSize, vectorSize); 
    samples2 = generateMixtureAllontanamento(2, sampleSize, vectorSize);
    
    h = ttest2(samples1, samples2);
    [p, h2] = ranksum(samples1, samples2);
        s = [s, strcat(type, " ",string(h == 1), " ",string(h2), " ", string(vectorSize))];

    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    vectorSize = 8;
    
    
    
    QR_REG_PARAM  = 0.01

    Sigma0 = randn(vectorSize);
    [Q, R] = qr(Sigma0);
    D = diag(abs(2*randn(1,vectorSize)) + QR_REG_PARAM);
    Sigma0 = Q * D * Q';
    
    Sigma1_temp = randn(vectorSize);
    [Q1, R] = qr(Sigma1_temp);
    Sigma1 = Q1 * Sigma0 * Q1';
    
    type = "multivariato_gaussiano_cambio_media";
    
    samples1 = mvnrnd(zeros(1, vectorSize), Sigma0, sampleSize);
    val = rand(1, vectorSize);
    val = val/norm(val);
    val = val/sqrt(vectorSize);
    samples2 = mvnrnd(val, Sigma0, sampleSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    type = "multivariato_gaussiano_cambio_varianza";
    
    samples1 = mvnrnd(zeros(1, vectorSize), Sigma0, sampleSize);
    samples2 = mvnrnd(zeros(1, vectorSize), 2*Sigma0, sampleSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," " ,string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    type = "multivariato_gaussiano_cambio_correlazione";
    
    samples1 = mvnrnd(zeros(1, vectorSize), Sigma0, sampleSize);
    samples2 = mvnrnd(zeros(1, vectorSize), Sigma1, sampleSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
        s = [s, strcat(type, " ",string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];

    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);

    
     type = "multivariato_uniforme_cambio_range";
    
    samples1 = -1 + (2*rand(sampleSize, vectorSize));
    samples2 = -2 + (4*rand(sampleSize, vectorSize));
    
    [pval, T2] = hotell2(samples1,samples2);

        s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];

    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
      type = "multivariato_mixture_cambio_media";
    
    samples1 = generateMixtureVecMedia(0, sampleSize, vectorSize);
    samples2 = generateMixtureVecMedia(1, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
        s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
     type = "multivariato_mixture_cambio_allontanamento";
    
    samples1 = generateMixtureVecAllont(1, sampleSize, vectorSize);
    samples2 = generateMixtureVecAllont(2, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    
    
    
    
    type = "protein_shift_media";
    file = 'protein_standardized.mat';
     samples1 = loadData(0, file, sampleSize, vectorSize);
    samples2 = loadData(1, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);

    type = "protein_rotated";
    
     samples1 = loadDataRotated(false, file, sampleSize, vectorSize);
    samples2 = loadDataRotated(true, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    
   
    
    
    
     type = "creditCards_shift_media";
    file = 'creditCards_standardized.mat';
     samples1 = loadData(0, file, sampleSize, vectorSize);
    samples2 = loadData(1, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);

    type = "creditCards_rotated";
    
     samples1 = loadDataRotated(false, file, sampleSize, vectorSize);
    samples2 = loadDataRotated(true, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    
    
    
    
    
    
     type = "sensors_shift_media";
    file = 'sensorDrive_standardized.mat';
     samples1 = loadData(0, file, sampleSize, vectorSize);
    samples2 = loadData(1, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);

    type = "sensors_rotated";
    
     samples1 = loadDataRotated(false, file, sampleSize, vectorSize);
    samples2 = loadDataRotated(true, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    
    
     type = "miniboone_shift_media";
    file = 'MiniBooNE_PID_standardized.mat';
     samples1 = loadData(0, file, sampleSize, vectorSize);
    samples2 = loadData(1, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);

    type = "miniboone_rotated";
    
     samples1 = loadDataRotated(false, file, sampleSize, vectorSize);
    samples2 = loadDataRotated(true, file, sampleSize, vectorSize);
    
    [pval, T2] = hotell2(samples1,samples2);
    
    s = [s, strcat(type," ", string(pval < 0.05), " ", string(pval < 0.05), " ", string(vectorSize))];
    saveD(char(strcat("./generationDir/", type, ".mat")), [samples1; samples2]);
    
    
    
    
    fileID = fopen("./generationDir/index.txt", "w+");
    t = size(s);
    v = t(2);
    for a = [1:v]
        fprintf(fileID, "%s\n", s(a));
    end
     fclose(fileID);
     
         
end

function ret = loadData(shift, fileName, sampleSize, vectorSize)
    ret = zeros(sampleSize, vectorSize);
    t = open(strcat('/home/massimo/datasets/',fileName));
    v = rand(1, 8);
    v = v / norm(v) * shift;
    for a = [1:sampleSize]
       ind = randi(size(t.dataset, 1));
       s = t.dataset(ind, :);
       s = s(1:vectorSize);
       ret(a, :) = v + s;
    end
end

function ret = loadDataRotated(rotated, fileName, sampleSize, vectorSize)
    ret = zeros(sampleSize, vectorSize);
    t = open(strcat('/home/massimo/datasets/',fileName));
    QR_REG_PARAM  = 0.01

    Sigma0 = randn(vectorSize);
    [Q, R] = qr(Sigma0);
    D = diag(abs(2*randn(1,vectorSize)) + QR_REG_PARAM);
    Sigma0 = Q * D * Q';
    
    for a = [1:sampleSize]
       ind = randi(size(t.dataset, 1));
       s = t.dataset(ind, :);
       s = s(1:vectorSize);
       if rotated
        ret(a, :) = Sigma0 * (s');
       else
         ret(a, :) = s;
       end
    end
end


function ret = generateMixtureShifMedia(shift, sampleSize, vectorSize)
    ret = zeros(sampleSize, 1);
    for a = [1:sampleSize]
        if (rand() < 0.5)
            ret(a, 1) = normrnd(-1 + shift, 1);
        else
            ret(a, 1) = normrnd(1 + shift, 1);
        end
    end
end

function ret = generateMixtureAllontanamento(shift, sampleSize, vectorSize)
    ret = zeros(sampleSize, 1);
    
    for a = [1:sampleSize]
        if (rand() < 0.5)
            ret(a, 1) = normrnd(shift, 1);
        else
            ret(a, 1) = normrnd(-shift, 1);
        end
    end
end

function ret = generateMixtureVecMedia(shift, sampleSize, vectorSize)
    ret = zeros(sampleSize, vectorSize);
    val = rand(1, vectorSize);
    val = val / norm(val);
    for a = [1:sampleSize]
        if (rand() < 0.5)
            ret(a, :) = mvnrnd((shift * val)- (ones(1, vectorSize)), eye(vectorSize));
        else
            ret(a, :) = mvnrnd((val * shift) + (ones(1, vectorSize)), eye(vectorSize));
        end
    end
end

function ret = generateMixtureVecAllont(shift, sampleSize, vectorSize)
    ret = zeros(sampleSize, vectorSize);
    
    for a = [1:sampleSize]
        if (rand() < 0.5)
            ret(a, :) = mvnrnd(shift*(ones(1, vectorSize)), eye(vectorSize));
        else
            ret(a, :) = mvnrnd(-shift*(ones(1, vectorSize)), eye(vectorSize));
        end
    end
end


function saveD(dest, sample)
    fileID = fopen(dest, 'w');
    s = size(sample);
    s2 = s(2);
    s = s(1);
    for a = [1:s]
        for b = [1:s2]
            fprintf(fileID, "%f ", sample(a, b));
        end;
            fprintf(fileID, "\n");
    end;
    fclose(fileID);
end
